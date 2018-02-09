// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define INF std::numeric_limits<int>::max()


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

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
	int num_zones, num_jobs;
	std::cin >> num_zones >> num_jobs;

	std::vector<int> ticket_cost(num_zones);
	for(int i=0; i<num_zones; i++){
		std::cin >> ticket_cost[i];
	}

	std::vector<int> job_reward(num_jobs);
	long sum_rewards = 0;
	for(int i=0; i<num_jobs; i++){
		std::cin >> job_reward[i];
		sum_rewards += job_reward[i];
	}

	std::vector<std::vector<int> > jobs_to_zones(num_jobs, std::vector<int>());
	for(int i=0; i<num_jobs; i++){
		int num_zones_for_current_job;
		std::cin >> num_zones_for_current_job;
		for(int j=0; j<num_zones_for_current_job; j++){
			int z;
			std::cin >> z;
			jobs_to_zones[i].push_back(z);
		}
	}

	int num_nodes = num_jobs+num_zones+2;
	Graph G(num_nodes);
	int source = num_nodes-2;
	int target = num_nodes-1;
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for(int i=0; i<num_jobs; i++){
		eaG.addEdge(source, i, job_reward[i]); // source to job edges of reward capacity

		for(int j=0; j<jobs_to_zones[i].size(); j++){ // jobs to zones edges of infinite capacity
			eaG.addEdge(i, num_jobs+jobs_to_zones[i][j], INF);
		}
	}

	for(int i=0; i<num_zones; i++){
		eaG.addEdge(num_jobs+i, target, ticket_cost[i]); // zones to target edges of ticket cost capacity
	}

	long flow = boost::push_relabel_max_flow(G, source, target); // max flow is equal to min cut; min cut = sum of cost of zone
																 // tickets bought and rewards of jobs not taken
	std::cout << sum_rewards - flow << std::endl; // Actual profit = sum of all job rewards - (sum of rewards of jobs not taken 
												  // + ticket cost of zones visited)
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
	return 0;
}