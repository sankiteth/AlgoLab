#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define INF std::numeric_limits<int>::max()

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,IK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, IK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK,Tds> Triangulation;

typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef std::pair<IK::Point_2, int> IPoint;

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

class Gang{
public:
	int where, who, how;
	Gang(int u, int v, int w): where(u), who(v), how(w) {}
};

class Agent{
public:
	IK::Point_2 p;
	int z;
	Agent(int x_, int y_, int z_): p( IK::Point_2(x_,y_) ), z(z_) {}
	bool operator<(Agent& a1){
		return this->z <= a1.z;
	}

	Agent() {
		this->z = INF;
	}
};

void testcase_LP(){
	int holmes_fee, min_where, min_who, min_how;
	std::cin >> holmes_fee >> min_where >> min_who >> min_how;

	int num_agents, num_gangs;
	std::cin >> num_agents >> num_gangs;

	std::vector<IPoint> g_loc;
	std::vector<Gang> g_info;
	for(int i=0; i<num_gangs; i++){
		int x,y,u,v,w;
		std::cin >> x >> y >> u >> v >> w;
		g_loc.push_back( IPoint(IK::Point_2(x,y), i) );
		g_info.push_back(Gang(u,v,w));
	}

	std::vector<Agent> a;
	for(int i=0; i<num_agents; i++){
		int x,y,z;
		std::cin >> x >> y >> z;
		a.push_back(Agent(x,y,z));
	}
	//std::sort(a.begin(), a.end());

	Triangulation t;
	t.insert(g_loc.begin(), g_loc.end());

	std::vector<int> closest_spy(num_gangs, -1);
	std::vector<int> spying(num_agents, -1);

	for(int i=0; i<num_agents; i++){ // find closest cheapest agent for every gang member
		Vertex_handle v = t.nearest_vertex(a[i].p);
		int clo_spy = closest_spy[v->info()];
		if(clo_spy == -1){
			closest_spy[v->info()] = i;
			spying[i] = v->info();
		} 
		else{
			if ( a[clo_spy].z > a[i].z ){
				closest_spy[v->info()] = i;
				spying[i] = v->info(); 
			}
		}
	}

	// Formulate LP. num variables = g : one for each gang member being spied. If gang member not spied, not considered as
	// variable. Number of constraints = 4
	Program lp(CGAL::LARGER, true, 0, false, 0);
	int var_index = 0;
	for(int i=0; i<num_agents; i++){
		int target_gang = spying[i];
		if(closest_spy[ target_gang ] != i){ // check if current agent spies the gang member
			continue;
		}

		lp.set_c(var_index, a[i].z); // build objective to minimize
		lp.set_a(var_index, 0, g_info[target_gang].where); // constriant for where
		lp.set_a(var_index, 1, g_info[target_gang].who); // constraint for who
		lp.set_a(var_index, 2, g_info[target_gang].how); // constriant for how
		lp.set_u(var_index, true, 24); // upper bound for number of hours for each watch is 24 hours
		var_index++;
	}

	lp.set_b(0, min_where);
	lp.set_b(1, min_who);
	lp.set_b(2, min_how);

	Solution sol = CGAL::solve_linear_program(lp, ET());
	//std::cout << "sol.objective_value()=" << CGAL::to_double( sol.objective_value() ) << std::endl;
	if (sol.is_infeasible() || sol.objective_value() > holmes_fee){ // not possible at lower or equal cost than holmes
		std::cout << "H\n";
	}
	else{
		std::cout << "L\n";
	}
}

void testcase_MCMF(){
	int holmes_fee, min_where, min_who, min_how;
	std::cin >> holmes_fee >> min_where >> min_who >> min_how;

	int num_agents, num_gangs;
	std::cin >> num_agents >> num_gangs;

	std::vector<IPoint> g_loc;
	std::vector<Gang> g_info;
	for(int i=0; i<num_gangs; i++){
		int x,y,u,v,w;
		std::cin >> x >> y >> u >> v >> w;
		g_loc.push_back( IPoint(IK::Point_2(x,y), i) );
		g_info.push_back(Gang(u,v,w));
	}

	std::vector<Agent> a;
	for(int i=0; i<num_agents; i++){
		int x,y,z;
		std::cin >> x >> y >> z;
		a.push_back(Agent(x,y,z));
	}
	//std::sort(a.begin(), a.end());

	Triangulation t;
	t.insert(g_loc.begin(), g_loc.end());

	std::vector<int> closest_spy(num_gangs, -1);
	std::vector<int> spying(num_agents, -1);

	for(int i=0; i<num_agents; i++){ // find closest cheapest agent for every gang member
		Vertex_handle v = t.nearest_vertex(a[i].p);
		int clo_spy = closest_spy[v->info()];
		if(clo_spy == -1){
			closest_spy[v->info()] = i;
			spying[i] = v->info();
		} 
		else{
			if ( a[clo_spy].z > a[i].z ){
				closest_spy[v->info()] = i;
				spying[i] = v->info(); 
			}
		}
	}

	Graph G(num_agents*2 + 3 + 2); // 3 nodes, 1 each for where, who, how
	int source = num_agents*2 + 3;
	int target = source+1;

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	int var_index = 0;
	for(int i=0; i<num_agents; i++){
		int target_gang = spying[i];
		if(closest_spy[ target_gang ] != i){ // check if current agent spies the gang member
			continue;
		}

		eaG.addEdge(i, i+num_agents, INF, a[i].z); // edge for cost of the agent
		eaG.addEdge(i+num_agents, 2*num_agents  , g_info[target_gang].where * 24, 0);  // edge for 'where' flow
		eaG.addEdge(i+num_agents, 2*num_agents+1, g_info[target_gang].who   * 24, 0);  // edge for 'who' flow
		eaG.addEdge(i+num_agents, 2*num_agents+2, g_info[target_gang].how   * 24, 0);  // edge for 'how' flow

		eaG.addEdge(source, i, INF, 0); // edge from source to agent
	}

	eaG.addEdge(2*num_agents  , target, min_where, 0); // edges from where, who and how to target
	eaG.addEdge(2*num_agents+1, target, min_who  , 0);
	eaG.addEdge(2*num_agents+2, target, min_how  , 0);

	boost::successive_shortest_path_nonnegative_weights(G, source, target);
    long cost = boost::find_flow_cost(G);

    OutEdgeIt e, eend;
    long s_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    //std::cout << "s-out flow " << s_flow << std::endl;

    if(s_flow < (min_where + min_who + min_how) ){
    	std::cout << "H1\n";
    	return;
    }

    //std::cout << "cost=" << cost << std::endl;

    long tot_cost = 0;
    for(int i=0; i<num_agents; i++){
    	int target_gang = spying[i];
		if(closest_spy[ target_gang ] != i){ // check if current agent spies the gang member
			continue;
		}

    	bool success; Edge edge;
    	boost::tie(edge, success) = boost::edge(i+num_agents, 2*num_agents  , G);
    	cost = capacitymap[edge]-rescapacitymap[edge];

    	boost::tie(edge, success) = boost::edge(i+num_agents, 2*num_agents+1, G);
    	cost = std::max(cost, capacitymap[edge]-rescapacitymap[edge] );

    	boost::tie(edge, success) = boost::edge(i+num_agents, 2*num_agents+2, G);
    	cost = std::max(cost, capacitymap[edge]-rescapacitymap[edge] );

    	cost *= a[i].z;
    	tot_cost += cost;
	}

	//std::cout << "tot_cost=" << tot_cost << std::endl;

    if(tot_cost <= holmes_fee){
    	std::cout << "L\n";
    }
    else{
    	std::cout << "H\n";
    }
}

int main(){
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase_MCMF();
	}

	return 0;
}