#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/config.hpp>

using namespace std;
using namespace boost;

 struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;

typedef adjacency_list<vecS, // out-edges 
		vecS, // vertices
		undirectedS, // directed, undirected or bidirectional
		no_property, // vertex property
		property < edge_component_t, std::size_t >, // edge property
		no_property // graph property
		> Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type

// Property map edge -> weight
//typedef property_map<Graph, edge_weight_t>::type	WeightMap;
//typedef property_map<Graph, vertex_distance_t>::type DistanceMap; // vertex_distance_t should be a property in the graph!!!

typedef graph_traits<Graph>::edge_iterator    EdgeIt;	// Edge iterator
typedef graph_traits<Graph>::vertex_iterator    VertexIt;	// Vertex iterator

void important_bridges()
{
	int m, n; cin >> n >> m;
	Graph g(n); // Creates an empty graph on V vertices
	//WeightMap weightmap = get(edge_weight, g);  // Interior Property Map
	for (int i = 0; i < m; ++i) 
	{
		int u, v;
		cin >> u >> v;
		Edge e;	bool success;	
		tie(e, success) = add_edge(u, v, g);
		//weightmap[e] = w;
	}

	property_map < Graph, edge_component_t >::type component = get(edge_component, g);
	std::size_t num_comps = biconnected_components(g, component);

	EdgeIt ei, ei_end;
	vector<int> num_edges_in_comp(n, 0);
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
	{
		num_edges_in_comp[component[*ei]] += 1;
	}

	vector<pair<int, int>> bridges;
	int num_bridges = 0;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
	{
		if (num_edges_in_comp[component[*ei]] == 1)
		{
			num_bridges++;
			pair<int, int> p; 
			if (source(*ei, g) < target(*ei, g))
			{
				p = make_pair(source(*ei, g), target(*ei, g));
			}
			else
			{
				p = make_pair(target(*ei, g), source(*ei, g));
			}

			bridges.push_back(p);
		}
	}

	sort(bridges.begin(), bridges.end(), [] (pair<int, int> x, pair<int, int> y) -> bool
											{
												if (x.first == y.first)
												{
													return x.second < y.second;
												}
												else
													return x.first < y.first;
											});

	cout << num_bridges << endl;
	for (int i = 0; i < bridges.size(); ++i)
	{
		cout << bridges[i].first << " " << bridges[i].second << endl;
	}
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		important_bridges();
	}
}