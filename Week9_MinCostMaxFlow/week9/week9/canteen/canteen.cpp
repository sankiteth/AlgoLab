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

void testcase(){
    int n; std::cin >> n;
    std::vector<int> a(n), c(n);
    for(int i=0; i<n; i++){
        std::cin >> a[i] >> c[i];
    }

    int max_price = MINUS_INF;
    int total_students = 0;
    std::vector<int> s(n), p(n);
    for(int i=0; i<n; i++){
        std::cin >> s[i] >> p[i];
        max_price = std::max(max_price, p[i]);
        total_students += s[i];
    }

    std::vector<int> v(n-1), e(n-1);
    for(int i=0; i<n-1; i++){
        std::cin >> v[i] >> e[i];
    }

    // Create Graph and Maps
    Graph G(n + 2);
    int source = n;
    int target = n+1;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    for(int i=0; i<n; i++){
        eaG.addEdge(source, i   , a[i], c[i]);             // source to menus
        eaG.addEdge(i     , target , s[i], max_price-1*p[i]); // menus to target, which is students

        if(i < n-1){
            eaG.addEdge(i, i+1, v[i], e[i]); // one day's menu to next day's menu (carried over supply)
        }
    }

    // Find the min cost max flow
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt ebeg, eend;
    int flow = 0; // Also, equals the number of sites sold
    for(boost::tie(ebeg, eend) = boost::out_edges(source, G); ebeg != eend; ++ebeg) {
        flow += (capacitymap[*ebeg] - rescapacitymap[*ebeg]);
    }

    cost = cost - (max_price*flow);
    int profit = -1 * cost;

    if(flow >= total_students){
        std::cout << "possible ";
    } else {
        std::cout << "impossible ";
    }

    std::cout << flow << " " << profit << std::endl;
}

int main(){
    int t; std::cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }
}