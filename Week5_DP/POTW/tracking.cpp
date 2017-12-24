#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <map>
#include <tuple>
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
typedef graph_traits<Graph>::out_edge_iterator    OutEdgeIt;	// Edge iterator
typedef graph_traits<Graph>::vertex_iterator    VertexIt;	// Vertex iterator

// Wrong!!!
int memoize(vector<vector<int> >& dp, vector<vector<int> >& visited, vector<vector<int> > solved, Graph& g, WeightMap& wmap, 
	map<Edge, int>& river, int cur_node, int num_rivers)
{
	cout << "current " << cur_node << " " << num_rivers << endl;
	if (dp[cur_node][num_rivers] != numeric_limits<int>::max())
	{
		cout << "return1: cur_node=" << cur_node << " num_rivers=" << num_rivers << " dp=" << dp[cur_node][num_rivers] << endl;
		return dp[cur_node][num_rivers];
	}

	visited[cur_node][num_rivers] = true;

	int min_dist = numeric_limits<int>::max();
	OutEdgeIt oeit, oeend;
	for (tie(oeit, oeend) = out_edges(cur_node, g); oeit != oeend; ++oeit) 
	{
		Edge e = *oeit;
		int v = (int)target(e, g);

		int new_num_rivers = ( (river[e] == 1) ? num_rivers-1 : num_rivers);
		int ret;

		if (solved[v][new_num_rivers])
		{
			ret = dp[v][new_num_rivers];
		}
		else
		{
			if (visited[v][new_num_rivers])
			{
				cout << "already visited: neighbor=" << v << " " << new_num_rivers << endl;
				continue;
			}

			ret = memoize(dp, visited, solved, g, wmap, river, v, new_num_rivers);
		}

		if (ret != numeric_limits<int>::max())
		{
			dp[v][new_num_rivers] = ret;
			min_dist = min(min_dist, wmap[e] + dp[v][new_num_rivers]);
		}
	}

	solved[cur_node][num_rivers] = true;
	dp[cur_node][num_rivers] = min_dist;
	cout << "return2: " << "cur_node=" << cur_node << " num_rivers=" << num_rivers << " dp=" << dp[cur_node][num_rivers] << endl;
	return dp[cur_node][num_rivers];
}

void tracking()
{
	int n,m,k,x,y; cin >> n >> m >> k >> x >> y;
	Graph g(n);
	map<Edge, int> river;
	WeightMap wmap = get(edge_weight, g);  // Interior Property Map
	for (int i = 0; i < m; ++i)
	{
		int a,b,c,d; cin >> a >> b >> c >> d;

		Edge e;	bool success;	
		tie(e, success) = add_edge(a, b, g);
		wmap[e] = c;
		river[e] = d;
	}

	vector<Vertex> predmap(n); // Exterior Property Map
	vector<int> distmap(n);    // Exterior Property Map

	dijkstra_shortest_paths(g, x,
			predecessor_map(make_iterator_property_map(			// named parameters
					predmap.begin(), get(vertex_index, g))).
			distance_map(make_iterator_property_map(			// concatenated by .
					distmap.begin(), get(vertex_index, g)))
			);

	vector<vector<int> > dp(n, vector<int>(k+1, numeric_limits<int>::max()));
	vector<vector<int> > visited(n, vector<int>(k+1, false));
	vector<vector<int> > solved(n, vector<int>(k+1, false));
	// base case
	for (int i = 0; i < n; ++i)
	{
		dp[i][0] = (distmap[i]);
	}

	int sol = memoize(dp, visited, solved, g, wmap, river, y, k);

	cout << sol << endl;
}

void tracking_1()
{
	int n,m,k,x,y; cin >> n >> m >> k >> x >> y;
	Graph g((k+1)*n);
	WeightMap wmap = get(edge_weight, g);

	for (int i = 0; i < m; ++i)
	{
		int a,b,c,d; cin >> a >> b >> c >> d;
		Edge e;	bool success;	
		tie(e, success) = add_edge(a, b, g);
		wmap[e] = c;

		// k layers of the same graph, the layers being connected only by river edges.
		for (int j = 1; j <= k; ++j)
		{	
			tie(e, success) = add_edge(a + n*j, b + n*j, g);
			wmap[e] = c;

			if (d == 1)
			{
				tie(e, success) = add_edge(a + n*(j-1), b + n*j, g);
				wmap[e] = c;
				tie(e, success) = add_edge(a + n*j, b + n*(j-1), g);
				wmap[e] = c;
			}
		}
	}

	vector<Vertex> predmap((k+1)*n);
	vector<int> distmap((k+1)*n);

	dijkstra_shortest_paths(g, x,
			predecessor_map(make_iterator_property_map(
					predmap.begin(), get(vertex_index, g))).
			distance_map(make_iterator_property_map(
					distmap.begin(), get(vertex_index, g)))
			);

	cout << distmap[y + n*k] << endl;
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		tracking_1();
	}
}