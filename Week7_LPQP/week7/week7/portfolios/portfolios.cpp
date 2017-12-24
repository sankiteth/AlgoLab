#include <iostream>
#include <cassert>
#include <cmath>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int m){
	int c[n], r[n];
	for(int i=0; i<n; i++){
		std::cin >> c[i] >> r[i];
	}

	int v[n][n];
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			std::cin >> v[i][j];
		}
	}

	int C[m], R[m], V[m];
	for(int i=0; i<m; i++){
		std::cin >> C[i] >> R[i] >> V[i];
	}

	for(int i=0; i<m; i++){
		Program lp (CGAL::SMALLER, true, 0, false, 0); // set lower bounds to 0, upper bounds to infinity

		int X[n]; // variables
		for(int j=0; j<n; j++){
			X[j] = j;
		}

		// Expected return and cost contraints
		for(int j=0; j<n; j++){
			lp.set_a(X[j], 0, -1 * r[j]);
			lp.set_a(X[j], 1, c[j]);
		}

		lp.set_b(0, -1 * R[i]);
		lp.set_b(1, C[i]);

		// objective function
		for(int r=0; r<n; r++){
			for(int c=0;c<=r; c++) { // on and below the diagonal
				lp.set_d(X[r], X[c], 2*v[r][c]);
			}
		}

		Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
		assert (s.solves_quadratic_program(lp));

		if (s.is_optimal() && s.objective_value()<=V[i]){
			std::cout << "Yes." << std::endl;
		} else {
			std::cout << "No." << std::endl;
		}
	}
}

int main(){
	int n, m;
	std::cin >> n >> m;
	while(n!=0 && m!=0){
		testcase(n, m);
		std::cin >> n >> m;
	}
}