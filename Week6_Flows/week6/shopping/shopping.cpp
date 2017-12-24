// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
// BGL includes
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

void shopping()
{
	int n, m, s; std::cin >> n >> m >> s;
	std::vector<int> vec_s(s);
	for (int i = 0; i < s; ++i)
	{
		std::cin >> vec_s[i];
	}

	Graph G(n+1);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	int source = 0;
	int target = n;

	for (int i = 0; i < m; ++i)
	{
		int u, v; std::cin >> u >> v;
		if (u == source)
		{
			eaG.addEdge(source, v, 1); // from u, to v, capacity 1
		}
		else if (v == source)
		{
			eaG.addEdge(source, u, 1); // from v, to u, capacity 1
		}
		else
		{
			eaG.addEdge(u, v, 1); // from u, to v, capacity 1
			eaG.addEdge(v, u, 1); // from v, to u, capacity 1
		}
	}

	for (int i = 0; i < vec_s.size(); ++i)
	{
		eaG.addEdge(vec_s[i], target, INF);
	}

	long flow1 = boost::push_relabel_max_flow(G, source, target);
	//long flow2 = boost::edmonds_karp_max_flow(G, source, target);

	//std::cerr << "flow1=" << flow1 << std::endl;
	if (flow1 >= vec_s.size())
	{
		std::cout << "yes" << std::endl;
	}
	else
	{
		std::cout << "no" << std::endl;
	}
}

int main()
{
	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
	{
		shopping();
	}
}