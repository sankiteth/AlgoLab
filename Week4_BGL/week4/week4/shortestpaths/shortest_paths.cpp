#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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
//typedef property_map<Graph, vertex_distance_t>::type DistanceMap; // vertex_distance_t should be a property in the graph!!!

typedef graph_traits<Graph>::edge_iterator    EdgeIt;	// Edge iterator
typedef graph_traits<Graph>::vertex_iterator    VertexIt;	// Vertex iterator

void shortest_path()
{
	int n,m,q; cin >> n >> m >> q; 

	Graph g(n);
	WeightMap w_map = get(edge_weight, g);
	for (int i = 0; i < n; ++i)
	{
		long int lon, lat; cin >> lon >> lat;
	}

	for (int i = 0; i < m; ++i)
	{
		int a, b, c; cin >> a >> b >> c;
		Edge e;
		bool success;
		tie(e, success) = add_edge(a, b, g);
		w_map[e] = c;
	}

	for (int i = 0; i < q; ++i)
	{
		int s, t; cin >> s >> t;
		if (s == t)
		{
			cout << 0 << endl;
			continue;
		}

		vector<Vertex> pred_map(n);
		vector<long int> dist_map(n, 0);

		dijkstra_shortest_paths(g, s,
			predecessor_map(make_iterator_property_map(
					pred_map.begin(), get(vertex_index, g))).
			distance_map(make_iterator_property_map(
					dist_map.begin(), get(vertex_index, g)))
			);

		if (pred_map[t] == t)
		{
			cout << "unreachable" << endl;
		}
		else
		{
			cout << dist_map[t] << endl;
		}
	}
}

int main()
{
	shortest_path();
}