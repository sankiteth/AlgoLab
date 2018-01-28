// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <math.h>
#include <algorithm>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

#define MAX_D 30

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct point{
	int x, y, z;
	point(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
	point(){}
};

int h, t;
std::vector<point> H;
std::vector<point> T;

bool solve_lp(int degree){
	Program lp (CGAL::LARGER, false, 0, false, 0);

	int var, delta;
	for(int b=0; b<h; b++){ // one constraint each for a healthy cell
		var = 0;
		for (int i = 0; i <= degree; i++) {
  			for (int j = 0; j <= degree; j++) {
    			for (int k = 0; k <= degree; k++) {
      				if (i+j+k > degree){ 
      					break;
      				}

      				double value = pow(H[b].x, i) * pow(H[b].y, j) * pow(H[b].z, k);
      				lp.set_a(var, b, ET(value));
      				var++;
    			}
  			}
		}

		delta = var;
		lp.set_b (b, delta);
	}

	for(int r=0; r<t; r++){ // one constraint each for a tumor cell
		var = 0;
		for (int i = 0; i <= degree; i++) {
  			for (int j = 0; j <= degree; j++) {
    			for (int k = 0; k <= degree; k++) {
      				if (i+j+k > degree){ 
      					break;
      				}

      				double value = pow(T[r].x, i) * pow(T[r].y, j) * pow(T[r].z, k);
      				lp.set_a(var, r+h, ET(value));
      				var++;
    			}
  			}
		}

		lp.set_b (r+h, 0);
		lp.set_r(r+h, CGAL::SMALLER);
	}

	// objective function: -delta (the solver minimizes)
	lp.set_c(delta, -1);
	// enforce a bounded problem:
	lp.set_u (delta, true, 1);

	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution s = CGAL::solve_linear_program(lp, ET(), options);

	return s.is_optimal() && (-s.objective_value() > 0);
}

void testcase(){
	std::cin >> h >> t;
	H = std::vector<point>(h);
	T = std::vector<point>(t);

	for(int i=0; i<h; i++){
		int x, y, z;
		std::cin >> x >> y >> z;
		H[i] = point(x, y, z);
	}

	for(int i=0; i<t; i++){
		int x, y, z;
		std::cin >> x >> y >> z;
		T[i] = point(x, y, z);
	}

	int low=0, high=MAX_D;
	int sol_degree = -1;

	// Find first point where solves successfully
	while(low <= high){
    	if (solve_lp(low)) {
			sol_degree = low;
      		high = low;
      		break;
    	} 
    	else {
      		low = std::max(1, 2 * low);
    	}
  	}

  	if (low != 2 && low != 1 && low != 0) { // if solved at any of these 3 points, then done
  		low = int(low / 2) + 1;
		while(low <= high){ // binary search for the smallest degree polynomial
			int mid = (low + high)/2;
	  		if(solve_lp(mid)){
	  			sol_degree = mid;
	  			high = mid-1;
	  			//std::cout << "high=" << high << std::endl;
	  		} else {
	  			low = mid + 1;
	  			//std::cout << "low=" << low << std::endl;
	  		}
		}
	}

	if(sol_degree != -1){
		std::cout << sol_degree << std::endl;
	}
	else{
		std::cout << "Impossible!" << std::endl;
	}

}

int main(){
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}