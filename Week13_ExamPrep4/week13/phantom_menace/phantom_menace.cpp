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

void testcase(){
	int n, m, s, d;
	std::cin >> n >> m >> s >> d;

	int num_nodes = 2*n + 2;
	int source = 2*n;
	int target = 2*n + 1;

	Graph G(num_nodes);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for(int i=0; i<n; i++){
		int node_in = i;
		int node_out = i+n;
		eaG.addEdge(node_in, node_out, 1);
	}

	for(int i=0; i<m; i++){
		int u, v;
		std::cin >> u >> v;
		int u_out = u+n;
		int v_in = v;
		eaG.addEdge(u_out, v_in, 1);
	}

	for(int i=0; i<s; i++){
		int v; std::cin >> v;
		eaG.addEdge(source, v, 1);
	}

	for(int i=0; i<d; i++){
		int u; std::cin >> u;
		eaG.addEdge(u+n, target, 1);
	}

	int flow = boost::push_relabel_max_flow(G, source, target);
	std::cout << flow << std::endl;
}

int main(){
	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}