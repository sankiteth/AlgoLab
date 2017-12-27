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

int white=0, gray=1, black=2;
Graph G;

int DFS_Visit(std::vector<std::vector<int> >& adj_list, int cur, int parent, std::vector<int>& stack, std::vector<int>& color,
				WeightMap& weightmap){
	int ret = -1;
	color[cur] = gray;
	stack.push_back(cur);

	for(auto it = adj_list[cur].begin(); it != adj_list[cur].end(); it++){
		int val = *it;
		if(color[val] == white){
			ret = DFS_Visit(adj_list, val, cur, stack, color, weightmap);
			if(ret != -1){ // cycle detected, so immediately return
				return ret;
			}
		} else if(color[val] == gray && val!=parent){ // back edge - cycle detected!
			int max_wt_in_cycle = MINUS_INF;
			for (int i=stack.size()-1; i>=0; i--){
				if(i < stack.size()-1){
					int u = stack[i+1];
					int v = stack[i];
					Edge edge;	
					bool success;	
					boost::tie(edge, success) = boost::edge(u, v, G);
					max_wt_in_cycle = std::max(max_wt_in_cycle, weightmap[edge]);
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

int DFS(std::vector<std::vector<int> >& adj_list, WeightMap& weightmap, int s, int n){
	std::vector<int> stack;
	std::vector<int> color(n, white);
	int max_wt_in_cycle = DFS_Visit(adj_list, s, -1, stack, color, weightmap);

	return max_wt_in_cycle;
}

void testcase(){
	int n, source;
	std::cin >> n >> source;

	G = Graph(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);

	for(int i=0; i<n-1; i++){
		for(int j=1; j<n-i; j++){
			int w; 
			std::cin >> w;

			Edge edge;	
			bool success;	
			boost::tie(edge, success) = boost::add_edge(i, i+j, G);
			weightmap[edge] = w;
		}
	}

	std::vector<Vertex> predmap(n); // Exterior Property Map
	std::vector<int> distmap(n);    // Exterior Property Map

	boost::prim_minimum_spanning_tree(G, &predmap[0],
		boost::distance_map(boost::make_iterator_property_map(			
				distmap.begin(), boost::get(boost::vertex_index, G))).
		root_vertex(source-1)
	);

	std::vector<std::vector<int> > used_edges(n, std::vector<int>(n, 0)); // edges used by the MST
	std::vector<std::vector<int> > adj_list(n, std::vector<int>());       // adj list representation of MST

	long total_wt = 0;
	for(int i=0; i<n; i++){
		total_wt += distmap[i];
	}

	for(int i=0; i<n; i++){
		if(i == predmap[i]){ // root of the tree
			continue;
		}
		used_edges[i][predmap[i]] = 1;
		used_edges[predmap[i]][i] = 1;

		adj_list[i].push_back(predmap[i]);
		adj_list[predmap[i]].push_back(i);
	}

	long min_wt_ST = INF;
	EdgeIt edge_beg, edge_end;
	for(boost::tie(edge_beg, edge_end) = boost::edges(G); edge_beg != edge_end; ++edge_beg){
		int source = boost::source(*edge_beg, G);
		int target = boost::target(*edge_beg, G);

		if(used_edges[source][target] == 0){ // edge not used in MST
			adj_list[source].push_back(target); // put the edge in the MST
			adj_list[target].push_back(source);

			int max_wt_in_cycle = DFS(adj_list, weightmap, source, n);

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

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
	{
		testcase();
	}
}