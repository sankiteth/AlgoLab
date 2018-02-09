// STL includes
#include <iostream>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define MINUS_INF std::numeric_limits<int>::min()
#define INF std::numeric_limits<int>::max()

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator
typedef boost::graph_traits<Graph>::edge_iterator  EdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

class pairs{
public:
	int b, s, c;
	pairs(int b_, int s_, int c_):b(b_), s(s_), c(c_) {}
};

void testcase(){
	int b, s, p;
	std::cin >> b >> s >> p;

	std::vector<std::vector<bool> > has_edge(b, std::vector<bool>(s, false));
    std::vector<pairs> ps;
    int max_c = MINUS_INF;
    for(int i=0; i<p; i++){
    	int u, v, w;
    	std::cin >> u >> v >> w;
    	ps.push_back(pairs(u, v, w) );
    	has_edge[u][v] = true;
    	max_c = std::max(max_c, w);
    }
	
	int extra_left = s;
	Graph G(b + s + extra_left + 2);
	int source = s + b + extra_left;
	int target = s + b + + extra_left + 1;

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    for(int i=0; i<p; i++){
    	eaG.addEdge(ps[i].b, b + ps[i].s, 1, -1*ps[i].c + max_c);
    }

    for(int i=0; i<extra_left; i++){
    	eaG.addEdge(b+s+i, b+i, 1, 0 + max_c); // dummy edges from extra-left to sailors. This ensures a perfect
    	eaG.addEdge(source, b+s+i, 1, 0);      // matching bet. extra-left and sailors. Now, replace those edges from this perfect
    }										   // with the edges in the optimal solution of the original problem.

    // source edges
    for(int i=0; i<b; i++){
    	eaG.addEdge(source, i, 1, 0);
    }

    // target edges
    for(int i=0; i<s; i++){
    	eaG.addEdge(b+i, target, 1, 0);
    }

    // Find the min cost max flow
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    long cost = boost::find_flow_cost(G);

    /*long flow = boost::push_relabel_max_flow(G, source, target);
    boost::cycle_canceling(G);
    long cost = boost::find_flow_cost(G);*/

    OutEdgeIt ebeg, eend;
    long flow = 0; // Also, equals the number of sites sold
    for(boost::tie(ebeg, eend) = boost::out_edges(source, G); ebeg != eend; ++ebeg) {
        flow += (capacitymap[*ebeg] - rescapacitymap[*ebeg]);
    }

    cost = cost - (max_c*flow);
    long spectacle = -1 * cost;
    std::cout << spectacle << std::endl;

}

int main(){
	std::ios_base::sync_with_stdio(true);

	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}