// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_set>
// BGL includes
#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define INF std::numeric_limits<int>::max()

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

int testcase()
{
	int w, n; std::cin >> w >> n;
	std::vector<std::tuple<int, int> > bricks;
	std::vector<int> cracks(w+1, 0);
	int n_old = n;
	std::unordered_set <std::tuple<int,int>, boost::hash<std::tuple<int,int> > > duplicate;
	for (int i = 0; i < n_old; ++i)
	{
		int u, v; std::cin >> u >> v;
		if (u == v) // Ignore zero width bricks
		{
			n--;
			continue;
		}
		if (u > v) // make sure u < v
		{
			std::swap(u, v);
		}
		
		if (duplicate.find(std::make_tuple(u, v)) == duplicate.end()) // duplicate bricks could be used only once
		{
			if (!(u == 0 && v == w)) // full bricks can be as many as possible
			{
				duplicate.insert(std::make_tuple(u, v));	
			}
			bricks.push_back(std::make_tuple(u, v));
			cracks[u] = 1; cracks[v] = 1;
		}
		else
		{
			n--;
		}
	}

	std::cerr << "Bricks:\n";
	for (int i = 0; i < n; ++i)
	{
		std::cerr << std::get<0>(bricks[i]) << " " << std::get<1>(bricks[i]) << std::endl;
	}

	// w=0 correspond to source and w=w correspond to target
	Graph G(n + 2*(w+1));
	int source = n + 2*0;
	int target = n + 2*w;
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	// 2 nodes for each crack and an edge between them
	std::cerr << "Edges:\n";
	for (int i = 1; i < w; ++i)
	{
		if (cracks[i] == 1)
		{
			eaG.addEdge(n + 2*i, n + 2*i + 1, 1);
			std::cerr << n + 2*i << " " << n + 2*i + 1 << std::endl;
		}
	}

	int j = 0;
	for (auto i = bricks.begin(); i != bricks.end(); ++i, ++j)
	{
		if (std::get<0>(*i) == 0) // for source
		{
			eaG.addEdge(source, j, 1);
			std::cerr << source << " " << j << std::endl;
		}
		else // for crack
		{
			eaG.addEdge(n + 2 * std::get<0>(*i) + 1, j, 1);
			std::cerr << n + 2 * std::get<0>(*i) + 1 << " " << j << std::endl;
		}

		if (std::get<1>(*i) == w)// for target
		{
			eaG.addEdge(j, target, 1);
			std::cerr << j << " " << target << std::endl;
		}
		else // for crack
		{
			eaG.addEdge(j, n + 2 * std::get<1>(*i), 1);
			std::cerr << j << " " << n + 2 * std::get<1>(*i) << std::endl;
		}
	}

	int flow = boost::push_relabel_max_flow(G, source, target);
	//long flow = boost::edmonds_karp_max_flow(G, source, target);
	std::cout << flow << std::endl;
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