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
#include <boost/graph/max_cardinality_matching.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef	boost::graph_traits<Graph>::edge_iterator EdgeIt;

inline bool is_not_hole(int i, int j, std::vector<std::vector<int> >& board){
	return board[i][j] == 1;
}

inline bool is_safe(int i, int j, int n){
	return (i>=0 && i<n && j>=0 && j<n);
}

// Find maximum independent set in a bipartite graph.
// left set -> white cells, right set -> black cells. A knight in a white cell never attacks any white cell.
// Edges from left to right, if a knight at a white cell attacks a black cell. 
// This implies: finding a maximum independent set
// means finding a set of vertices such that knights placed on them do not attack each other, which is what we need.
int testcase(){
	int n; std::cin >> n;
	std::vector<std::vector<int> > board(n, std::vector<int>(n)); // assume top left corner is always white
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			std::cin >> board[i][j];
		}
	}

	int num_holes = 0;
	int num_nodes = n*n;
	Graph G(num_nodes);
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(board[i][j] == 0){
				num_holes++;
				continue;
			}

			if(is_safe(i-2, j-1, n) && is_not_hole(i-2, j-1, board)) boost::add_edge(i*n + j, (i-2)*n + (j-1), G);
			if(is_safe(i-2, j+1, n) && is_not_hole(i-2, j+1, board)) boost::add_edge(i*n + j, (i-2)*n + (j+1), G);
			if(is_safe(i-1, j-2, n) && is_not_hole(i-1, j-2, board)) boost::add_edge(i*n + j, (i-1)*n + (j-2), G);
			if(is_safe(i-1, j+2, n) && is_not_hole(i-1, j+2, board)) boost::add_edge(i*n + j, (i-1)*n + (j+2), G);
			if(is_safe(i+1, j-2, n) && is_not_hole(i+1, j-2, board)) boost::add_edge(i*n + j, (i+1)*n + (j-2), G);
			if(is_safe(i+1, j+2, n) && is_not_hole(i+1, j+2, board)) boost::add_edge(i*n + j, (i+1)*n + (j+2), G);
			if(is_safe(i+2, j-1, n) && is_not_hole(i+2, j-1, board)) boost::add_edge(i*n + j, (i+2)*n + (j-1), G);
			if(is_safe(i+2, j+1, n) && is_not_hole(i+2, j+1, board)) boost::add_edge(i*n + j, (i+2)*n + (j+1), G);
		}
	}

	/*EdgeIt ebeg, eend;
	std::cout << "Edges:\n";
	for(boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
		std::cout << *ebeg << std::endl;
	}*/

	// Do maximum matching in O(mn*ackerman(m,n)) time
	std::vector<Vertex> matemap(num_nodes);
        boost::edmonds_maximum_cardinality_matching(G,
            boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, G)));

    int max_matching_size = boost::matching_size(G, boost::make_iterator_property_map(
							matemap.begin(), boost::get(boost::vertex_index, G)));

    int max_independent_set_size = (num_nodes- num_holes) - max_matching_size; // (num_nodes- num_holes) is the actual number of nodes

    std::cout << max_independent_set_size << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}