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

void testcase(){
    int N, M, S;
    std::cin >> N >> M >> S;
    std::vector<int> l(S);
    for(int i=0; i<S; i++){
        std::cin >> l[i];
    }

    std::vector<int> s(M);
    for(int i=0; i<M; i++){
        std::cin >> s[i];
    }

    std::vector<std::vector<int> > b(N, std::vector<int>(M));
    int max_bid = MINUS_INF;
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            std::cin >> b[i][j];
            max_bid = std::max(max_bid, b[i][j]);
        }
    }

    // Create Graph and Maps
    Graph G(N+M+S+2);
    int source = N+M+S;
    int target = N+M+S+1;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Add edges to the graph
    for(int i=0; i<N; i++){
        eaG.addEdge(source, i, 1, 0); // Edge from source to all bidders.
        for(int j=0; j<M; j++){
            eaG.addEdge(i, j+N, 1, max_bid - b[i][j]); // the bid negated to act as cost, then max value added to every bid to 
                                                       // to avoid negative cost on edges.
        }
    }

    // Add edges from sites to states, to which the site belongs
    for(int i=0; i<M; i++){
        eaG.addEdge(i+N, N+M+(s[i]-1), 1, 0);
    }

    // Add edges from states to target
    for(int i=0; i<S; i++){
        eaG.addEdge(i+N+M, target, l[i], 0); // capacity is number of sells allowed in the state
    }

    // Find the min cost max flow
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    int flow = 0; // Also, equals the number of sites sold
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        flow += (capacitymap[*e] - rescapacitymap[*e]);
    }

    cost = cost - (max_bid*flow);
    int profit = -1 * cost;

    std::cout << flow << " " << profit << std::endl;
}

int main(){
    int t; std::cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }
}