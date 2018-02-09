#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property> Graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                                boost::property<boost::edge_weight_t,int> > DiGraph; // An edge-weighted digraph.
typedef boost::property_map<DiGraph, boost::edge_weight_t >::type WeightMap;
typedef boost::graph_traits<DiGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<DiGraph>::edge_descriptor Edge;

void testcase(){
    // Read the input and build graph G.
    int N, M, A, S, C, D; // Capital letters used for the input sizes.
    std::cin >> N >> M >> A >> S >> C >> D;

    DiGraph G(N);
    WeightMap weight_map = boost::get(boost::edge_weight, G);

    for (int m = 0; m < M; ++m) { // Lower case letters used for the counters.
        char w; 
        int x, y, z;
        std::cin >> w >> x >> y >> z;
        Edge e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(x,y,G);
        weight_map[e] = z;
        if (w == 'L') { // Add the reverse edge only for ski lifts.
            boost::tie(e, boost::tuples::ignore) = boost::add_edge(y,x,G);
            weight_map[e] = z;
        }
    }

    // Compute one distance map per agent.
    std::vector<std::vector<int> > distmap(A, std::vector<int>(N));
    for (int a = 0; a < A; ++a) {
        int i; std::cin >> i;
        boost::dijkstra_shortest_paths(G, i,
            boost::distance_map(boost::make_iterator_property_map(distmap[a].begin(),
                                                                        boost::get(boost::vertex_index, G))));
    }

    // Represent G’ as the pairwise distance matrix T from agents to shelters.
    std::vector<std::vector<int> > T(A, std::vector<int>(S));
    for (int s = 0; s < S; s++) {
        int j; std::cin >> j;
        for(int a = 0; a < A; a++) {
            T[a][s] = distmap[a][j]; // Unreachable pairs are at distance INT_MAX.
        }
    }

    // Binary search for the smallest t.
    int low = 0, high = INT_MAX;
    int sol; // It is guarenteed that all agents do make it to shelters
    while (low <= high) {
        int mid = low + (high-low)/2;
        // Build the bipartite matching graph G’’_t.
        Graph GG(A + C * S);
        for (int a = 0; a < A; ++a) {
            for (int s = 0; s < S; ++s) {
                if (T[a][s] == INT_MAX) 
                    continue; // Ignore unreachable shelters.
                for (int c = 0; c < C; ++c) { // c agents will still follow.
                    if (T[a][s] + (c + 1) * D <= mid) { // Agent can enter in time.
                        boost::add_edge(a, A + c * S + s, GG); // Add the edge to G’’_t.
                    }   
                }
            }
        }

        // Compute the size of the maximum cardinality matching.
        std::vector<Vertex> matemap(A + C * S);
        boost::edmonds_maximum_cardinality_matching(GG,
            boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, GG)));
        
        const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
        int matching_size = 0;
        for (int a = 0; a < A; ++a) { // Count the number of matched agents.
            matching_size += (matemap[a] != NULL_VERTEX);
        }
        if (matching_size == A) { // The agents all make it in time.
            sol = mid;
            high = mid-1;
        } else { // Some agents do not make it, so we need more time.
            low = mid+1;
        }
    }

    std::cout << sol << std::endl;
}

int main(){
    int t; std::cin >> t;
    for(int i=0; i<t; i++){
        testcase();
    }
}