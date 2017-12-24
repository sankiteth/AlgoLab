#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <utility>

// Boost includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#define INF std::numeric_limits<int>::max()

// BGL typedefs
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, int,
		boost::property<boost::edge_residual_capacity_t, int,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;

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
	int n, m;
	std::cin >> n >> m;

	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for(int i=0; i<m; i++){
		int from, to, c;
		std::cin >> from >> to >> c;
		eaG.addEdge(from, to, c);
	}

	// Find min cut via max flow
	int minmaxflow = INF;
	std::pair<int, int> source_sink;

	// Vertex i is in my group, and vertex (i+1) in friend's. O(nm^2) * n
	for (int i = 0; i < n; ++i) {
		int maxflow = boost::push_relabel_max_flow(G, i, (i+1)%n);
		if (maxflow < minmaxflow) {
			source_sink = std::make_pair(i,(i+1)%n);
			minmaxflow = maxflow;
		}
	}

	// BFS to find vertices reachable from source vertex in residual graph
	minmaxflow = push_relabel_max_flow(G, source_sink.first, source_sink.second); // rescapacity needs to get recalculatedsss
	std::vector<int> vis(n, false);
	std::queue<int> Q;
	vis[source_sink.first] = true; // Mark the source as visited
	Q.push(source_sink.first);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]){
				continue;
			}

			vis[v] = true;
			Q.push(v);
		}
	}

	// Output
	std::cout << minmaxflow << std::endl << std::count(vis.begin(), vis.end(), true);
	for (int i = 0; i < n; ++i) {
		if (vis[i]){
			std::cout << " " << i;
		}
	}

	std::cout << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}