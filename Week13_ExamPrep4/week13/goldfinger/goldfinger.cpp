#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <vector>
#include <utility>
#include <climits>

#define INF std::numeric_limits<double>::max()

typedef CGAL::Gmpq ET;
//typedef double ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef K::Point_2 P;
typedef std::vector<std::pair<P, double> > VP;

int num_sens, num_mpe, num_hench, max_inten;

bool solve_lp(int mid, std::vector<std::vector<ET> >& sens_mpe_dist, VP& sensors){
	Program lp(CGAL::LARGER, true, 0, false, 0);

		for(int i=0; i<=mid; i++){ // objective function, one variable each for the internsity of each mpe
			lp.set_c(i, 1);
		}

		// one constraint for each sensor
		for(int i=0; i<num_sens; i++){
			for(int j=0; j<=mid; j++){
				lp.set_a(j, i, sens_mpe_dist[i][j]);
			}

			lp.set_b(i, sensors[i].second);
		}

		Solution sol = CGAL::solve_nonnegative_linear_program(lp, ET());
		return sol.is_optimal() && (sol.objective_value() <= max_inten);
}

void testcase(){
	std::cin >> num_sens >> num_mpe >> num_hench >> max_inten;

	VP sensors;
	sensors.reserve(num_sens);
	for(int i=0; i<num_sens; i++){
		P p; int E;
		std::cin >> p >> E;
		sensors.push_back(std::make_pair(p, E));
	}

	VP mpes;
	mpes.reserve(num_mpe);
	for(int i=0; i<num_mpe; i++){
		P p; std::cin >> p;
		mpes.push_back(std::make_pair(p, INF));
	}

	std::vector<P> hench;
	for(int i=0; i<num_hench; i++){
		P p; std::cin >> p;
		hench.push_back(p);
	}

	// compute DT of the henchmen
	Delaunay d;
	d.insert(hench.begin(), hench.end());

	// compute max effect radius for MPEs
	for(int i=0; i<num_mpe; i++){
		if(num_hench > 0){
			Delaunay::Vertex_handle v = d.nearest_vertex(mpes[i].first);
			mpes[i].second = CGAL::squared_distance(mpes[i].first, v->point());
		}
	}

	std::vector<std::vector<ET> > sens_mpe_dist(num_sens, std::vector<ET>(num_mpe));
	// pre-compute sensor-to-mpe distances
	for(int i=0; i<num_sens; i++){
		for(int j=0; j<num_mpe; j++){
			K::FT sq_dist = CGAL::squared_distance(sensors[i].first, mpes[j].first);
			if(sq_dist < mpes[j].second){// if no henchmen gets signal
				sens_mpe_dist[i][j] = 1/ET(sq_dist);
			}
			else{
				sens_mpe_dist[i][j] = ET(0);
			}
		}
	}

	int low=0, high = num_mpe-1;
	int mid, sol_num = -1;

	// Find first point where solves successfully
	while(low <= high){
    	if (solve_lp(low, sens_mpe_dist, sensors)) {
			sol_num = low;
      		high = low;
      		break;
    	} 
    	else {
      		low = std::max(1, 2 * low);
    	}
  	}

  	if (low != 2 && low != 1 && low != 0) { // if solved at any of these 3 points, then done
  		low = int(low / 2) + 1;

		while(low<=high){
			mid = low + (high-low)/2;
			
			if (solve_lp(mid, sens_mpe_dist, sensors)){ // possible with current number of mpes
				sol_num = mid;
				high = mid-1;
			}
			else
			{
				low = mid+1;
			}
		}
	}

	if(sol_num == -1){
		std::cout << "impossible\n";
	}
	else{
		std::cout << sol_num+1 << std::endl;
	}

}

int main(){
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}