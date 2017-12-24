#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <map>
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

void ant_challenge()
{
	int n, e, s, a, b; cin >> n >> e >> s >> a >> b;
	vector < std::map < Edge, int > > std_species(s);

	Graph g(n);
	//WeightMap weightmap = get(edge_weight, g);
	for (int i = 0; i < e; ++i) 
	{
		int u, v; cin >> u >> v;
		Edge edge;	
		bool success;	
		tie(edge, success) = add_edge(u, v, g);

		for (int j = 0; j < s; ++j)
		{
			int s_j; cin >> s_j;
			std_species[j][edge] = s_j;
		}
	}

	vector < boost::associative_property_map < std::map < Edge, int > > > boost_species;
	for (int i = 0; i < s; ++i)
	{
  		boost::associative_property_map < std::map < Edge, int > > boost_spec(std_species[i]);
  		boost_species.push_back(boost_spec);
	}

	vector<int> h(s);
	for (int i = 0; i < s; ++i)
	{
		int h_i; cin >> h_i;
		h.push_back(h_i);
	}

	//print_graph(g);

	std::map<pair<int, int>, int> final_edges; //store the best paths here

	for (int i = 0; i < s; ++i)
	{
		vector<Vertex> predmap(n); // Exterior Property Map
		vector<int> distmap(n);    // Exterior Property Map

		prim_minimum_spanning_tree(g, &predmap[0],
				distance_map(make_iterator_property_map(			
						distmap.begin(), get(vertex_index, g))).
				weight_map(boost_species[i]).
				root_vertex(h[i])
				);

		for (int j = 0; j < n; ++j)
		{
			if (j == predmap[j]) // root of the tree
			{
				continue;
			}

			std::map<pair<int, int>, int>::iterator it = final_edges.find(make_pair(j,predmap[j]));
			if (it == final_edges.end())
			{
				final_edges[make_pair(j,predmap[j])] = distmap[j];
			}
			else
			{
				if (it->second > distmap[j])
				{
					final_edges[it->first] = distmap[j];
				}
			}
		}

		/*cout << "distmap\n";
		for (int i = 0; i < n; ++i)
		{
			cout << distmap[i] << endl;
		}*/
	}

	/*for (std::map<pair<int, int>, int>::iterator it = final_edges.begin(); it != final_edges.end(); ++it)
	{
		cout << (it->first).first << " " << (it->first).second << " " << (it->second) << endl;
	}*/

	g.clear();
	g = Graph(n);
	WeightMap weightmap = get(edge_weight, g);
	for (std::map<pair<int, int>, int>::iterator it = final_edges.begin(); it != final_edges.end(); ++it)
	{
		Edge edge;	
		bool success;	
		tie(edge, success) = add_edge((it->first).first, (it->first).second, g);
		weightmap[edge] = it->second;
	}

	vector<Vertex> predmap(n); // Exterior Property Map
	vector<int> distmap(n);    // Exterior Property Map

	dijkstra_shortest_paths(g, a,
			predecessor_map(make_iterator_property_map(			// named parameters
					predmap.begin(), get(vertex_index, g))).
			distance_map(make_iterator_property_map(			// concatenated by .
					distmap.begin(), get(vertex_index, g)))
			);

	cout << distmap[b] << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		ant_challenge();
	}
}