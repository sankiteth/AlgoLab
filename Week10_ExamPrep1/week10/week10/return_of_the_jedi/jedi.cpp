#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#define INF       std::numeric_limits<long>::max()
#define MINUS_INF std::numeric_limits<int>::min()

typedef boost::adjacency_list<boost::vecS, // out-edges 
		boost::vecS, // vertices
		boost::undirectedS, // directed, undirected or bidirectional
		boost::no_property, // vertex property
		boost::property<boost::edge_weight_t, int>, // edge property
		boost::no_property // graph property
		> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;	// Edge type

// Property map edge -> weight
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

typedef boost::graph_traits<Graph>::edge_iterator    EdgeIt;	// Edge iterator
typedef boost::graph_traits<Graph>::vertex_iterator    VertexIt;	// Vertex iterator

typedef std::vector<int> V;
typedef std::vector<V> VV;

int white=0, gray=1, black=2;
Graph G;
int total_count = 0;

int DFS_Visit(VV& adj_list, int cur, int parent, V& stack, V& color, VV& edge_weights){
	int ret = -1;
	color[cur] = gray;
	stack.push_back(cur);

	for(auto it = adj_list[cur].begin(); it != adj_list[cur].end(); it++){
		int val = *it;
		if(color[val] == white){
			ret = DFS_Visit(adj_list, val, cur, stack, color, edge_weights);
			if(ret != -1){ // cycle detected, so immediately return
				return ret;
			}
		} 
		else if(color[val] == gray && val != parent){ // back edge - cycle detected!
			int max_wt_in_cycle = MINUS_INF;
			for (int i=stack.size()-1; i>=0; i--){
				if(i < stack.size()-1){
					int u = stack[i+1];
					int v = stack[i];
					max_wt_in_cycle = std::max(max_wt_in_cycle, edge_weights[u][v]);
				}
				if(stack[i] == val){
					return max_wt_in_cycle;
				}
			}
		}
	}

	stack.pop_back();
	color[cur] = black;

	return ret;
}

int DFS(VV& adj_list, VV& edge_weights, int s, int n){
	std::vector<int> stack;
	std::vector<int> color(n, white);
	int max_wt_in_cycle = DFS_Visit(adj_list, s, -1, stack, color, edge_weights);

	return max_wt_in_cycle;
}

// O(VE)
void testcase(){
	int n, source;
	std::cin >> n >> source;

	G = Graph(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	std::vector<std::vector<int> > edge_weights(n, std::vector<int>(n, -1));

	for(int i=0; i<n-1; i++){
		for(int j=1; j<n-i; j++){
			int w; 
			std::cin >> w;

			Edge edge;	
			bool success;	
			boost::tie(edge, success) = boost::add_edge(i, i+j, G);
			weightmap[edge] = w;
			edge_weights[i][i+j] = edge_weights[i+j][i] = w; // needed for fast look up of edge weights. The property maps are slow!!!!
		}
	}

	std::vector<Vertex> predmap(n); // Exterior Property Map
	std::vector<int> distmap(n);    // Exterior Property Map

	boost::prim_minimum_spanning_tree(G, &predmap[0],
		boost::distance_map(boost::make_iterator_property_map(			
				distmap.begin(), boost::get(boost::vertex_index, G))).
		root_vertex(source-1)
	);

	std::vector<std::vector<int> > adj_list(n, std::vector<int>());       // adj list representation of MST

	long total_wt = 0;
	for(int i=0; i<n; i++){
		total_wt += distmap[i];
	}

	for(int i=0; i<n; i++){
		if(i == predmap[i]){ // root of the tree
			continue;
		}

		adj_list[i].push_back(predmap[i]);
		adj_list[predmap[i]].push_back(i);
	}

	long min_wt_ST = INF;
	EdgeIt edge_beg, edge_end;
	for(boost::tie(edge_beg, edge_end) = boost::edges(G); edge_beg != edge_end; ++edge_beg){
		int source = boost::source(*edge_beg, G);
		int target = boost::target(*edge_beg, G);

		if(predmap[source]!=target && predmap[target]!=source){ // edge not used in MST
			adj_list[source].push_back(target); // put the edge in the MST
			adj_list[target].push_back(source);

			int max_wt_in_cycle = DFS(adj_list, edge_weights, source, n);

			long new_min_wt_ST = total_wt - max_wt_in_cycle + weightmap[*edge_beg];
			min_wt_ST = std::min(min_wt_ST, new_min_wt_ST);

			if(new_min_wt_ST == total_wt){ // already a spanning tree with total weight equal to MST
				break;
			}

			adj_list[source].pop_back(); // remove the edges from the MST
			adj_list[target].pop_back();
		}
	}

	std::cout << min_wt_ST << std::endl;
}

int DFS_max_edge_visit(VV& adj_list, int source, int cur, int prev, VV& max_weight_edge, V& color, VV& edge_weights){
	color[cur] = gray;
	if(prev != -1){
		int w = edge_weights[prev][cur];

		if(w > max_weight_edge[source][prev]){
			max_weight_edge[source][cur] = w;
			max_weight_edge[cur][source] = w;
		} else {
			max_weight_edge[source][cur] = max_weight_edge[source][prev];
			max_weight_edge[cur][source] = max_weight_edge[source][prev];
		}
	}

	for(auto it = adj_list[cur].begin(); it != adj_list[cur].end(); it++){
		int val = *it;
		if(color[val] == white){
			DFS_max_edge_visit(adj_list, source, val, cur, max_weight_edge, color, edge_weights);
		}
	}

	color[cur] = black;
}

void DFS_max_edge(VV& adj_list, VV& edge_weights, VV& max_weight_edge, int n){
	for(int i=0; i<n-1; i++){
		std::vector<int> color(n, white);
		DFS_max_edge_visit(adj_list, i, i, -1, max_weight_edge, color, edge_weights);
	}
}

// O(V^2 + E)
void approach_2(){
	int n, source;
	std::cin >> n >> source;

	G = Graph(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	std::vector<std::vector<int> > edge_weights(n, std::vector<int>(n, -1));

	for(int i=0; i<n-1; i++){
		for(int j=1; j<n-i; j++){
			int w; 
			std::cin >> w;

			Edge edge;	
			bool success;	
			boost::tie(edge, success) = boost::add_edge(i, i+j, G);
			weightmap[edge] = w;
			edge_weights[i][i+j] = edge_weights[i+j][i] = w; // needed for fast look up of edge weights. The property maps are slow!!!!
		}
	}

	std::vector<Vertex> predmap(n); // Exterior Property Map
	std::vector<int> distmap(n);    // Exterior Property Map

	boost::prim_minimum_spanning_tree(G, &predmap[0],
		boost::distance_map(boost::make_iterator_property_map(			
				distmap.begin(), boost::get(boost::vertex_index, G))).
		root_vertex(source-1)
	);

	std::vector<std::vector<int> > adj_list(n, std::vector<int>());       // adj list representation of MST

	long total_wt = 0;
	for(int i=0; i<n; i++){
		total_wt += distmap[i]; // total weight of the minimum spanning tree
	}

	for(int i=0; i<n; i++){
		if(i == predmap[i]){ // root of the tree
			continue;
		}

		adj_list[i].push_back(predmap[i]);
		adj_list[predmap[i]].push_back(i);
	}

	std::vector<std::vector<int> > max_weight_edge(n, std::vector<int>(n, -1)); // edge of max wt between any nodes in the MST
	DFS_max_edge(adj_list, edge_weights, max_weight_edge, n);


	long min_wt_ST = INF;
	EdgeIt edge_beg, edge_end;
	for(boost::tie(edge_beg, edge_end) = boost::edges(G); edge_beg != edge_end; ++edge_beg){
		int source = boost::source(*edge_beg, G);
		int target = boost::target(*edge_beg, G);

		if(predmap[source]!=target && predmap[target]!=source){ // edge not used in MST
			int max_wt_in_cycle = max_weight_edge[source][target];
			long new_min_wt_ST = total_wt - max_wt_in_cycle + weightmap[*edge_beg];

			min_wt_ST = std::min(min_wt_ST, new_min_wt_ST);

			if(new_min_wt_ST == total_wt){ // already a spanning tree with total weight equal to MST
				break;
			}
		}
	}

	std::cout << min_wt_ST << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
	{
		approach_2();
	}
}