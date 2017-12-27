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
#include <boost/graph/dijkstra_shortest_paths.hpp>

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

void addDijikstraEdge(int u, int v, int w, Graph& G, EdgeWeightMap& w_map){
        Edge e;
        bool success;
        boost::tie(e, success) = boost::add_edge(u, v, G);
        w_map[e] = w;
    }

void testcase(){
    int n, m, source, target;
    std::cin >> n >> m >> source >> target;

    Graph G = Graph(n+1);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    Graph G_dijikstra = Graph(n);
    EdgeWeightMap weightMapDijikstra = boost::get(boost::edge_weight, G_dijikstra);

    long total_source_width = 0;

    for(int i=0; i<m; i++){
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        eaG.addEdge(a, b, c, d); // length of street is the cost, so that min cost path leads to shortest distance path
        eaG.addEdge(b, a, c, d); // bidirectional
        if(a == source || b == source) {
            total_source_width += c;
        }

        addDijikstraEdge(a, b, d, G_dijikstra, weightMapDijikstra);
        addDijikstraEdge(b, a, d, G_dijikstra, weightMapDijikstra);
    }

    std::vector<Vertex> pred_map(n);
    std::vector<long> dist_map(n, 0);

    boost::dijkstra_shortest_paths(G_dijikstra, source,
        predecessor_map(boost::make_iterator_property_map(
                                pred_map.begin(), get(boost::vertex_index, G_dijikstra))).
        distance_map(boost::make_iterator_property_map(
                                dist_map.begin(), get(boost::vertex_index, G_dijikstra)))
    );

    long dist = dist_map[target];

    //std::cout << "dist= " << dist << std::endl;

    int super_source = n;
    eaG.addEdge(super_source, source, 0, 0);

    long l = 1, r = total_source_width;
    //std::cout << "total_source_width=" << total_source_width << std::endl;
    long ans = 0;
    while(l<=r){
        long mid = (l+r)/2; 

        OutEdgeIt ebeg, eend;
        for(boost::tie(ebeg, eend) = boost::out_edges(super_source, G); ebeg != eend; ++ebeg) {
            capacitymap[*ebeg] = mid; // increase or decrease capacity
        }

        boost::successive_shortest_path_nonnegative_weights(G, super_source, target);
        long cost = boost::find_flow_cost(G);

        int flow = 0; // Also, equals the number of total athletes participating
        for(boost::tie(ebeg, eend) = boost::out_edges(super_source, G); ebeg != eend; ++ebeg) {
            flow += (capacitymap[*ebeg] - rescapacitymap[*ebeg]);
        }

        long shortest_path_cost = flow * dist;
        /*std::cout << "@@@@@: \n" << "shortest_path_cost=" << shortest_path_cost << "\nflow=" << flow << "\nmid=" << mid
                    << "\ncost=" << cost << std::endl;*/
        if(cost > shortest_path_cost){
            r = mid-1;
        } else if(cost < shortest_path_cost){
            l = mid+1;
        } else {
            l = mid+1;
            ans = flow;
            if(flow < mid){ // already total flow pushed in at super-source exceeded maximum flow
                break;
            }
        }
    }

    std::cout << ans << std::endl;
}

int main(){
    int t; std::cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }
}