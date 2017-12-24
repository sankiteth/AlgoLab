// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

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

void coin_tossing()
{
	int n, m; std::cin >> n >> m;
	std::vector<int> points(n, 0);
	std::vector<int> a, b;
	for (int i = 0; i < m; ++i)
	{
		int a_i, b_i, c_i; std::cin >> a_i >> b_i >> c_i;
		if (c_i == 0)
		{
			a.push_back(a_i);
			b.push_back(b_i);
		}
		else if (c_i == 1)
		{
			points[a_i] += 1;
		}
		else
		{
			points[b_i] += 1;
		}
	}

	std::vector<int> s(n);
	int sum = 0;
	for (int i = 0; i < n; ++i)
	{
		std::cin >> s[i];
		sum += s[i];
	}

	// total outflow from source != total inflow at target
	if (sum != m)
	{
		std::cout << "no" << std::endl;
		std::cerr << sum << std::endl;
		return;
	}

	// some player already has more points
	for (int i = 0; i < n; ++i)
	{
		if (s[i] < points[i])
		{
			std::cout << "no" << std::endl;
			std::cerr << "here2" << std::endl;
			return;
		}
	}

	// Build the graph for flow algorithm
	int undecided = a.size();
	std::cerr << undecided << std::endl;
	int numNodes = undecided + n + 2;
	int source = undecided + n;
	int target = source + 1;

	Graph G(numNodes);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for (int i = 0; i < undecided; ++i)
	{
		// both a and b can win this round. Whoever wins gets 1 point;
		eaG.addEdge(i, a[i] + undecided, 1); // from match, to player, capacity
		eaG.addEdge(i, b[i] + undecided, 1);
		eaG.addEdge(source, i, 1); // from source, to match, capacity
	}

	for (int i = 0; i < n; ++i)
	{
		eaG.addEdge(undecided+i, target, s[i] - points[i]); // from player, to target, points still needed by player
	}

	long flow1 = boost::push_relabel_max_flow(G, source, target);
	//long flow2 = boost::edmonds_karp_max_flow(G, source, target);

	std::cerr << "flow1=" << flow1 << std::endl;
	if (flow1 == undecided)
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
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
	{
		coin_tossing();
	}
}