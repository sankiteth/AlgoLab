// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define INF std::numeric_limits<long>::max()

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >   Graph;
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type        EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type   ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type     ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor           Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor         Edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    ReverseEdgeMap  &revedgemap;

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
    int num_nodes, num_edges, num_ps, num_photos;
    std::cin >> num_nodes >> num_edges >> num_ps >> num_photos;

    std::vector<int> ps(num_ps);
    for(int i=0; i<num_ps; i++){
        std::cin >> ps[i];
    }

    std::vector<int> photos(num_photos);
    for(int i=0; i<num_photos; i++){
        std::cin >> photos[i];
    }

    std::vector<std::pair<int, int> > edges(num_edges);
    for(int i=0; i<num_edges; i++){
        int x, y;
        std::cin >> x >> y;
        edges[i] = std::make_pair(x,y);
    }

    Graph G(num_nodes*2 + 2);
    int source = num_nodes*2;
    int target = source+1;

    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    for(int i=0; i<num_edges; i++){
        eaG.addEdge(edges[i].first, edges[i].second, INF); // on-going edges
        eaG.addEdge(num_nodes + edges[i].first, num_nodes + edges[i].second, 1); // returning edges
    }

    for(int i=0; i<num_photos; i++){
        eaG.addEdge(photos[i], num_nodes + photos[i], 1); // edges for photographs
    }

    for(int i=0; i<num_ps; i++){ // police stations as both source and sink
        eaG.addEdge(source, ps[i], 1);
        eaG.addEdge(num_nodes + ps[i], target, 1);
    }

    long flow = boost::push_relabel_max_flow(G, source, target);

    std::cout << flow << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);

    int t; std::cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }
}