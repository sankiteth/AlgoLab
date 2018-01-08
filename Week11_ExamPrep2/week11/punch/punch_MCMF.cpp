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
#define INF std::numeric_limits<double>::max()

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
    int n, k;
    std::cin >> n >> k;

    int c[n], v[n];
    for(int i=0; i<n; i++){
        std::cin >> c[i] >> v[i];
    }


    int max_flow = MINUS_INF;
    double min_cost = INF;
    for(int test_flow=1; test_flow <= n; test_flow++){

        Graph G((2*k+1)*n + 2 + 1);
        int source = 0;
        int target = (2*k+1)*n + 1;
        int super_source = target+1;

        EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
        EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
        ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
        ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

        for(int i=0; i<n; i++){
            eaG.addEdge(source, i*(2*k+1), 1, 0); // from source edges

            for(int j=0; j < 2*k+1; j++){
                int to = i*(2*k+1) + j;

                if(j-v[i] >= 0){
                    eaG.addEdge(to-v[i], to, n, c[i]); // horizontal edge
                }

                if(i>0){
                    eaG.addEdge(to-(2*k+1), to, n, 0); // vertical edge
                }

                if(j >= k){ // to sink edges
                    eaG.addEdge(to, target, 1, 0); // to sink edges
                }
            }
        }

        eaG.addEdge(super_source, source, test_flow, 0);

        // Find the min cost max flow
        boost::successive_shortest_path_nonnegative_weights(G, super_source, target);
        double cost = boost::find_flow_cost(G);

        // Iterate over all edges leaving the source to sum up the flow values.
        OutEdgeIt ebeg, eend;
        int flow = 0; // Also, equals the number of sites sold
        for(boost::tie(ebeg, eend) = boost::out_edges(super_source, G); ebeg != eend; ++ebeg) {
            flow += (capacitymap[*ebeg] - rescapacitymap[*ebeg]);
        }

        //std::cout << "cost=" << cost << " flow=" << flow << std::endl;

        if(cost <= min_cost){
            min_cost = cost;
            max_flow = flow;
        }
    }

    std::cout << min_cost << " " << max_flow << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);

    int t; 
    std::cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }
}