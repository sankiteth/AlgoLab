#include <iostream>
#include <set>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
using namespace boost;
using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms.
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
property<edge_capacity_t, long,
property<edge_residual_capacity_t, long,
property<edge_reverse_t, Traits::edge_descriptor,
property <edge_weight_t, long> > > > > Graph;
// Interior Property Maps.
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

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

struct mission {
    int x, y, q;
};

void testcase(){
    int n, m, l;
    cin>> n >> m >> l;
    vector<mission> mis;
    int max_q = INT_MIN;
    for(int i=0; i<m; i++){
        int x, y, q;
        cin >> x >> y >> q;
        mis.push_back({x,y,q});
        max_q = max(max_q, q);
    }

    Graph G(n+2);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    int source = n;
    int target = n+1;
    for(int i=0; i<n-1; i++){
        eaG.addEdge(i, i+1, l, max_q); // in-between stations edges
    }

    eaG.addEdge(source, 0, l, max_q);
    eaG.addEdge(n-1, target, l, max_q);

    for(int i=0; i<m; i++){
        eaG.addEdge(mis[i].x, mis[i].y, 1, max_q*(mis[i].y - mis[i].x) - mis[i].q);
    }

    // Run the min cost max flow algorithm.
    successive_shortest_path_nonnegative_weights(G, source, target);
    int flow = 0; // Iterate over all edges leaving the source to sum up the flow.
    OutEdgeIt e, eend;
    for(tie(e, eend) = out_edges(vertex(source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }

    // Undo the offset of the flow costs.
    int cost = (flow*max_q*(n+1))-find_flow_cost(G);
    cout << cost << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }

    return 0;
}