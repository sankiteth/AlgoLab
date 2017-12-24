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

void testcase_wrong()
{
	int l, p; std::cin >> l >> p;
	int total_soldiers = 0;
	std::vector<int> g(l), d(l);
	for (int i = 0; i < l; ++i)
	{
		std::cin >> g[i] >> d[i];
		total_soldiers += g[i];
	}

	// 2 nodes to extend for demand supply, 2 for max min capacities
	Graph G(l+5);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	std::vector<int> incoming_sum(l+5, 0), outgoing_sum(l+5, 0);
	std::vector<int> mini(p), maxi(p);
	int mini_sum = 0;
	for (int i = 0; i < p; ++i)
	{
		int f, t; std::cin >> f >> t >> mini[i] >> maxi[i];
		eaG.addEdge(f, t, maxi[i]-mini[i]);
		//std::cerr << "Edge: " << f << " " << t << " " << maxi-mini << std::endl;
		incoming_sum[t] += mini[i];
		outgoing_sum[f] += mini[i];
		mini_sum += mini[i];
	}

	// make source and sink for the demand supply situation
	int source = l;
	int target = l+1;
	int required_flow = 0;
	for (int i = 0; i < l; ++i)
	{
		if (g[i] > d[i]) // supply
		{
			// edge 
			eaG.addEdge(source, i, g[i] - d[i]); // min is 0
			//std::cerr << "Edge: " << source << " " << i << " " << 0 << std::endl;
			/*incoming_sum[i] += (g[i] - d[i]);
			outgoing_sum[source] += (g[i] - d[i]);
			mini_sum += (g[i] - d[i]);*/
		}
		else if (g[i] < d[i]) // demand
		{
			eaG.addEdge(i, target, total_soldiers - (d[i] - g[i])); // max is equal to total_soldiers
			required_flow += (d[i] - g[i]);
			//std::cerr << "Edge: " << i << " " << target << " " << 0 << std::endl;
			incoming_sum[target] += (d[i] - g[i]);
			outgoing_sum[i] += (d[i] - g[i]);
			mini_sum += (d[i] - g[i]);
		}
	}

	eaG.addEdge(l+2, source, 0);
	incoming_sum[source] += required_flow;
	outgoing_sum[l+2] += required_flow;
	mini_sum += required_flow;

	//std::cerr << "Edge: " << target << " " << source << " " << "INF" << std::endl;

	source = l+3;
	target = l+4;
	// make source and sink for min and max capacities
	for (int i = 0; i <= l+2; ++i)
	{
		// edge from new source to each node with sum of min capacities of incoming edges
		if (incoming_sum[i] > 0)
		{
			eaG.addEdge(source, i, incoming_sum[i]);
			//std::cerr << "Edge: " << source << " " << i << " " << incoming_sum[i] << std::endl;
		}
		// edge from node to new target with sum of min capacities of outgoing edges
		if (outgoing_sum[i] > 0)
		{
			eaG.addEdge(i, target, outgoing_sum[i]);
			//std::cerr << "Edge: " << i << " " << target << " " << outgoing_sum[i] << std::endl;
		}
	}

	// important!!! 
	// edge of infinite capacity from nodes target to source
	eaG.addEdge(l+1, l+2, INF);

	long flow1 = boost::push_relabel_max_flow(G, source, target);
	//long flow2 = boost::edmonds_karp_max_flow(G, source, target);

	//std::cerr << mini_sum << " " << flow1 << std::endl;
	if (flow1 == mini_sum)
	{
		// feasible flow exists
		std::cout << "yes" << std::endl;
	}
	else
	{
		std::cout << "no" << std::endl;
	}
}

std::vector<int> incoming_sum, outgoing_sum;
int mini_sum;

inline void add_edge(EdgeAdder& eaG, int from, int to, int mini, int maxi)
{
	eaG.addEdge(from, to, maxi - mini);
	incoming_sum[to] += mini;
	outgoing_sum[from] += mini;
	mini_sum += mini;
}

void testcase_1()
{
	int l, p; std::cin >> l >> p;
	// 4 nodes to extend for demand supply, 2 for max min capacities
	Graph G(l+4);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	std::vector<int> g(l), d(l);
	int g_sum = 0, d_sum = 0;
	for (int i = 0; i < l; ++i)
	{
		std::cin >> g[i] >> d[i];
		g_sum += g[i];
		d_sum += d[i];
	}

	if (d_sum > g_sum)
	{
		std::cout << "no" << std::endl;
		return;
	}

	incoming_sum = std::vector<int>(l+4, 0);
	outgoing_sum = std::vector<int>(l+4, 0);
	mini_sum = 0;
	int s_supply = l, t_demand = l+1, source = l+2, target = l+3;
	int minimum, maximum;
	for (int i = 0; i < l; ++i)
	{
		add_edge(eaG, s_supply, i, 0, g[i]); // For supply
		add_edge(eaG, i, t_demand, d[i], g_sum+d[i]); // For demand
	}

	int mini, maxi;
	for (int i = 0; i < p; ++i)
	{
		int f, t; std::cin >> f >> t >> mini >> maxi;
		add_edge(eaG, f, t, mini, maxi);
	}

	for (int i = 0; i <= l+3; ++i) // make source and sink for min and max capacities
	{
		if (incoming_sum[i] > 0) // edge from new source to each node with sum of min capacities of incoming edges
		{
			eaG.addEdge(source, i, incoming_sum[i]);
		}

		if (outgoing_sum[i] > 0) // edge from node to new target with sum of min capacities of outgoing edges
		{
			eaG.addEdge(i, target, outgoing_sum[i]);
		}
	}

	// important!!! 
	// edge of infinite capacity from nodes target to source, of the the demand supply situation
	eaG.addEdge(t_demand, s_supply, INF);

	long flow = boost::push_relabel_max_flow(G, source, target);
	if (flow == mini_sum)
	{
		// feasible flow exists
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
		testcase_1();
	}
}