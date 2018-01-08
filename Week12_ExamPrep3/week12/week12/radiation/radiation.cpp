// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <math.h>
#include <algorithm>

#define MAX_D 30

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct point{
	int x, y, z;
	point(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
	point(){}
};

inline bool operator==(const point& e, const point& f) 
	{ return e.x == f.x && e.y == f.y && e.z && f.z; }

bool contains(std::vector<point>& v, point p){
	for(int i=0; i<v.size(); i++){
		if(v[i].x == p.x && v[i].y == p.y && v[i].z == p.z){
			return true;
		}
	}

	return false;
}

bool dummy(int degree, int ans){
	if(degree >= ans){
		return true;
	}
	else{
		return false;
	}
}

void testcase(){
	int h, t; std::cin >> h >> t;
	std::vector<point> H(h);
	std::vector<point> T(t);

	for(int i=0; i<h; i++){
		int x, y, z;
		std::cin >> x >> y >> z;
		H[i] = point(x, y, z);
	}

	bool overlap = false;
	for(int i=0; i<t; i++){
		int x, y, z;
		std::cin >> x >> y >> z;
		T[i] = point(x, y, z);

		if(contains(H, T[i])){
			overlap = true;
		}
	}

	if(overlap){
		std::cout << "Impossible!\n";
		return;
	}

	if(h==0 || t==0){
		std::cout << "1\n";
		return;
	}

	/*for(int i=0; i<h; i++){
		std::cout << H[i].x << H[i].y << H[i].z << std::endl;
	}

	for(int i=0; i<t; i++){
		std::cout << T[i].x << T[i].y << T[i].z << std::endl;
	}*/

	int delta;
	int low=1, high=MAX_D+1;
	while(low<high){ // binary search for the smallest degree polynomial

		int degree = (low + high)/2;
		std::cout << "degree=" << degree << std::endl;
		Program lp (CGAL::SMALLER, false, 0, false, 0);

		int var;
		for(int b=0; b<h; b++){ // one constraint each for a healthy cell
			var = 0;
			for(int d=0; d <= degree; d++){      		// total degree of the polynomial
				for(int i=0; i <= d; i++){       	    // power of x = i
					int powers_left = d-i;
					for(int j=0; j <= powers_left; j++){    // power of y = j, power of z = power_left-j
						// set up <= constraint for point outside polynomial(ex, d=1):
	    				//   alpha x + beta y + eta z + gamma >= delta
						lp.set_a (var, b, -1 * (int)pow(H[b].x, i) * (int)pow(H[b].y, j) * (int)pow(H[b].z, powers_left-j) );
						var++;
					}
				}
			}

			delta = var;
			lp.set_a (var, b, 1);    // delta
			lp.set_b (b, 0);
		}

		for(int r=0; r<t; r++){ // one constraint each for a tumor cell
			var = 0;
			for(int d=0; d <= degree; d++){      		// total degree of the polynomial
				for(int i=0; i <= d; i++){       	    // power of x = i
					int powers_left = d-i;
					for(int j=0; j <= powers_left; j++){    // power of y = j, power of z = power_left-j
						// set up <= constraint for point inside polynomial(ex, d=1):
	    				//  alpha x + beta y + eta z + gamma <= 0  
						lp.set_a (var, r+h, (int)pow(T[r].x, i) * (int)pow(T[r].y, j) * (int)pow(T[r].z, powers_left-j) );
						var++;
					}
				}
			}

			lp.set_b (r+h, 0);
		}

		// objective function: -delta (the solver minimizes)
  		lp.set_c(delta, -1);
  		// enforce a bounded problem:
  		lp.set_u (delta, true, 1);

  		// solve the program, using ET as the exact type
  		Solution s = CGAL::solve_linear_program(lp, ET());
  		//assert (s.solves_linear_program(lp));


  		//std::cout << s.objective_value() << std::endl;

		// check optimality
  		if (s.is_optimal() && (s.objective_value() < 0)) {
  			high = degree;
  		} else {
  			low = degree + 1;
  		}
	}

	if(high<=MAX_D){
		std::cout << high << std::endl;
	}
	else{
		std::cout << "Impossible!\n";
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