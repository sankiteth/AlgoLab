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
		undirectedS, // directed, undirected or bidirectional
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

void print_graph(Graph g)
{
	VertexIt vert_beg, vert_end;
	EdgeIt edge_beg, edge_end;

	cout << "Vertices\n";
	for(tie(vert_beg, vert_end) = vertices(g); vert_beg != vert_end; ++vert_beg)
		cout << *vert_beg << endl;

	cout << "Edges\n";
	for(tie(edge_beg, edge_end) = edges(g); edge_beg != edge_end; ++edge_beg)
		cout <<  *edge_beg << endl;
}

void print_tree(vector<int> p)
{
	for (size_t i = 0; i != p.size(); ++i)
    if (p[i] != i)
      cout << "parent[" << i << "] = " << p[i] << endl;
    else
      cout << "parent[" << i << "] = no parent" << endl;
}

void func()
{
	int n, m; cin >> n >> m;

	Graph g(n); // Creates an empty graph on V vertices
	WeightMap weightmap = get(edge_weight, g);  // Interior Property Map
	for (int i = 0; i < m; ++i) 
	{
		int u, v, w;		// Each edge: <from> <to> <weight>
		cin >> u >> v >> w;
		Edge e;	bool success;	
		tie(e, success) = add_edge(u, v, g);
		weightmap[e] = w;
	}

	if (n==1 || n==0)
	{
		cout << 0 << " " << 0 << endl;
		return;
	}

	//print_graph(g);

	// records the edges (p[u],u) for all u in V of MST
	vector<Vertex> predmap(n); // Exterior Property Map
	vector<int> distmap(n);    // Exterior Property Map

    // 0 will be the root of the MST.
    // un-named parameter version
    /*prim_minimum_spanning_tree(g, 0, &p[0], distance, weightmap, indexmap, 
     default_dijkstra_visitor());*/

	// named parameter version
    prim_minimum_spanning_tree(g, &predmap[0],
			distance_map(make_iterator_property_map(			// concatenated by .
					distmap.begin(), get(vertex_index, g))).
			root_vertex(0)
			);

    //print_tree(p);

    int sum = 0;
    for (int i = 1; i < n; ++i)
    {
    	sum += distmap[i];
    }

    // Wrong!!! The farthest node in MST does not guarantee farthest node in the grpah!!!
    // Find farthest node from node 0 using Dijkstra
    
	dijkstra_shortest_paths(g, 0,
			predecessor_map(make_iterator_property_map(			// named parameters
					predmap.begin(), get(vertex_index, g))).
			distance_map(make_iterator_property_map(			// concatenated by .
					distmap.begin(), get(vertex_index, g)))
			);

	int max_dist_from_0 = distmap[0];
	for (int i = 1; i < n; ++i)
	{
		if (distmap[i] > max_dist_from_0)
		{
			max_dist_from_0 = distmap[i];
		}
	}

    cout << sum << " " << max_dist_from_0 << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		func();
	}

	return 0;
}