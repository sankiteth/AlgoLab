#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

#define MINUS_INF numeric_limits<int>::min()

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

// O(n^5)
void testcase_dp(){
	int n; cin >> n;
	vector<vector<int> > a = vector<vector<int>  >(n+1, vector<int>(n+1));

	for(int i=1; i<=n; i++){
		for(int j=1; j<=n; j++){
			cin >> a[i][j];
		}
	}

	int num_rows = 2;
	long dp[num_rows][n+1][n+1] = {}; // dp [r][i][j]: Max collected till row r, with first path ending at col i, second at col j, i<j
							   // r=0 for even rows, 1 for odd rows; counting starts from 1

	// base case for first row
	//cout << "First row:\n";
	for(int i=1; i<n; i++){
		for(int j=1; j<=n; j++){
			dp[1][i][j] = dp[1][i][j-1] + a[1][j]; // first row of maze, only 2nd path collects
			//cout << dp[1][i][j] << " ";
		}
	}

	//cout << endl;

	for(int r=2; r<=n; r++){           // Each row of the maze
		long ps[n+1][n+1] = {};
		for(int i=1; i<=n; i++){
			for(int j=i; j<=n; j++){
				ps[i][j] = ps[i][j-1] + a[r][j];
			}
		}

		for(int i=1; i<n; i++){        // First path
			for(int j=i+1; j<=n; j++){ // Second path

				long max_rij = MINUS_INF;
				for(int k1=1; k1<=i; k1++){
					for(int k2=i+1; k2<=j; k2++){
						max_rij = max(max_rij, dp[(r-1)%2][k1][k2] + ps[k1][i] + ps[k2][j]);
					}
				}

				dp[r%2][i][j] = max_rij;
			}
		}
	}

	cout << dp[n%2][n-1][n] << endl;
}

// O(f * (E + VlogV)), where f=flow value, E=num_edges=O(n^2) here; V=num_nodes=O(n^2) 
void testcase_mcmf(){
	int n; cin >> n;
	vector<vector<int> > maze = vector<vector<int>  >(n, vector<int>(n));
	int max_coins = MINUS_INF;
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cin >> maze[i][j];
			max_coins = max(max_coins, maze[i][j]);
		}
	}

	int num_nodes = n*n;
	Graph G = Graph(2*num_nodes);
	int source = 0;
	int target = 2*num_nodes - 1;

    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    for(int i=0; i<n; i++){
    	for(int j=0; j<n; j++){
    		int cur_node = (i*n) + j;
    		int twin_node = num_nodes + cur_node;
    		eaG.addEdge(cur_node, twin_node, 1, -maze[i][j] + max_coins); // carries the coin in the cell
    		eaG.addEdge(cur_node, twin_node, 1, 0           + max_coins); // does not carry the coin in the cell

    		if(j<n-1){ // edge to the right cell
    			eaG.addEdge(twin_node, i*n + (j+1), 2, 0);
    		}

    		if(i<n-1){ // edge to the below cell
    			eaG.addEdge(twin_node, (i+1)*n + j, 2, 0);
    		}
    	}
    }

    // Find the min cost max flow
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt ebeg, eend;
    int flow = 0; // Always 2 units of flow from source to traget, one for each pass
    for(boost::tie(ebeg, eend) = boost::out_edges(source, G); ebeg != eend; ++ebeg) {
        flow += (capacitymap[*ebeg] - rescapacitymap[*ebeg]);
    }

    cost = cost - ( flow * max_coins*(2*n-1) ); // all paths from source to target has length 2*n-1. Each such length increases
    int profit = -1 * cost;                     // the cost by max_coins.

    std::cout << profit << std::endl;
}

int main(){
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase_mcmf();
	}
}