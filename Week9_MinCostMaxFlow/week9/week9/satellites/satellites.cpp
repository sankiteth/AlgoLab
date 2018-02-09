// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <utility>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::bidirectionalS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			    EdgeIt;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;
typedef	boost::graph_traits<Graph>::in_edge_iterator			InEdgeIt;

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
	int g, s, l;
	std::cin >> g >> s >> l;

	int source = g + s;
	int target = g + s + 1;

	std::vector<std::pair<int, int> > links(l);
	for(int i=0; i<l; i++){
		std::cin >> links[i].first >> links[i].second;
	}

	Graph G(g+s+2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for(int i=0; i<l; i++){
		eaG.addEdge(links[i].first, links[i].second + g, 1);
	}

	for(int i=0; i<g; i++){
		eaG.addEdge(source, i, 1);
	}

	for(int i=0; i<s; i++){
		eaG.addEdge(i+g, target, 1);
	}

	long flow = boost::push_relabel_max_flow(G, source, target);

	/*// BFS to find vertices reachable from source vertex in residual graph
	std::vector<int> vis(g+s+2, false); // visited flags
	std::queue<int> Q;
	vis[source] = true; // Mark the source as visited
	Q.push(source);
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
	}*/

	// Find minimum vertex cover now, from maximum matching found above, using Konig's algorithm
	int left_visited = 0;
	int right_visited = 0;

	std::queue<int> Q_left;
	std::queue<int> Q_right;
	std::vector<int> new_vis(g+s+2, false);
	OutEdgeIt ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::out_edges(source, G); ebeg != eend; ++ebeg) {
		if(rescapacitymap[*ebeg] == 1){            // if residual capacity of the edge is not zero, then edge is not part of 
			const int v = boost::target(*ebeg, G); // maximum matching, hence v is unmatched vertex.
			new_vis[v] = true;        
			Q_left.push(v);
			left_visited++;
		}
	}

	bool is_left = true;
	while(!( Q_left.empty() && Q_right.empty() )) {
		if(is_left) { // in left set
			while(!Q_left.empty()) {
				int left = Q_left.front();
				Q_left.pop();
				OutEdgeIt ebeg, eend;
				for (boost::tie(ebeg, eend) = boost::out_edges(left, G); ebeg != eend; ++ebeg) {
					const int v = boost::target(*ebeg, G);
					if(rescapacitymap[*ebeg] == 1 && v != source){ // edge not in maximum matching
						if(!new_vis[v]) { // if not visited till now
							new_vis[v] = true; // make node from right set visited
							Q_right.push(v);
							right_visited++;
						}
					}
				}
			}
		} else { // in right set
			while(!Q_right.empty()) {
				int right = Q_right.front();
				Q_right.pop();
				InEdgeIt ebeg, eend;
				for (boost::tie(ebeg, eend) = boost::in_edges(right, G); ebeg != eend; ++ebeg) {
					const int u = boost::source(*ebeg, G);
					if(rescapacitymap[*ebeg] == 0 && u != target){ // edge in maximum matching
						if(!new_vis[u]) { // if not visited till now
							new_vis[u] = true; // make node from left set visited
							Q_left.push(u);
							left_visited++;
						}
					}
				}
			}
		}
	
		is_left = !is_left; // flip from left to right and vice-versa
	}

	std::cout << g - left_visited << " " << right_visited << std::endl;

	for(int i=0; i<g; i++){ // unvisited in left
		if(!new_vis[i]){
			std::cout << i << " ";
		}
	}

	for(int i=0; i<s; i++){ // visited in right
		if(new_vis[i+g]){
			std::cout << i << " ";
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