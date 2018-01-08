#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define INF numeric_limits<long>::max()

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, // out-edges 
		vecS, // vertices
		directedS, // directed, undirected or bidirectional
		no_property, // vertex property
		property<edge_weight_t, int>, // edge property
		no_property // graph property
		> Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type

// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

typedef graph_traits<Graph>::edge_iterator    EdgeIt;	// Edge iterator
typedef graph_traits<Graph>::vertex_iterator    VertexIt;	// Vertex iterator

inline  int Node(int i,int j) { 
	return ((i-1)*i)/2 + j;
}

void testcase(){
	int k; cin >> k;
	vector<vector<int> > balls(k+1, vector<int>(k+1));
	for(int i=1; i<=k; i++){
		for(int j=1; j<=i; j++){
			cin >> balls[i][j];
		}
	}

	int num_balls = (k*(k+1))/2;
	int num_nodes = num_balls + 1;
	Graph G(num_nodes);
	WeightMap weightmap = get(edge_weight, G);

	for(int i=1; i<=k; i++){
		for(int j=1; j<=i; j++){
			int cur_node = Node(i, j);
			int cur_wt = balls[i][j];
			Edge edge;	
			bool success;

			// edges to nodes in previous layer
			if(i-1>0){
				if(j-1>0){
					tie(edge, success) = add_edge(cur_node, Node(i-1, j-1), G);
					weightmap[edge] = cur_wt;
				}

				if(j<=i-1){
					tie(edge, success) = add_edge(cur_node, Node(i-1, j), G);
					weightmap[edge] = cur_wt;
				}
			}

			// edge to next node of current layer
			if(j-1>0){
				tie(edge, success) = add_edge(cur_node, Node(i, j-1), G);
				weightmap[edge] = cur_wt;
			}
			if(j+1<=i){
				tie(edge, success) = add_edge(cur_node, Node(i, j+1), G);
				weightmap[edge] = cur_wt;
			}

			// edges to nodes in next layer
			if(i+1<=k){
				tie(edge, success) = add_edge(cur_node, Node(i+1, j), G);
				weightmap[edge] = cur_wt;

				tie(edge, success) = add_edge(cur_node, Node(i+1, j+1), G);
				weightmap[edge] = cur_wt;
			}
		}
	}

	int top = Node(1, 1); 
	int left = Node(k,1); 
	int right = Node(k ,k);

	vector<Vertex> predmap_top(num_nodes);
	vector<int> distmap_top(num_nodes);

	dijkstra_shortest_paths(G, top,
			predecessor_map(make_iterator_property_map(
					predmap_top.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					distmap_top.begin(), get(vertex_index, G)))
			);

	vector<Vertex> predmap_left(num_nodes);
	vector<int> distmap_left(num_nodes);

	dijkstra_shortest_paths(G, left,
			predecessor_map(make_iterator_property_map(
					predmap_left.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					distmap_left.begin(), get(vertex_index, G)))
			);


	vector<Vertex> predmap_right(num_nodes);
	vector<int> distmap_right(num_nodes);

	dijkstra_shortest_paths(G, right,
			predecessor_map(make_iterator_property_map(
					predmap_right.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					distmap_right.begin(), get(vertex_index, G)))
			);

	long min_wt = INF;
	for(int i=1; i<=k; i++){
		for(int j=1; j<=i; j++){
			int cur_node = Node(i, j);
			long new_wt = distmap_top[cur_node] + distmap_left[cur_node] + distmap_right[cur_node] + balls[i][j];
			min_wt = min(min_wt, new_wt);
		}
	}

	cout << min_wt << endl;
}

int main(){
	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}
