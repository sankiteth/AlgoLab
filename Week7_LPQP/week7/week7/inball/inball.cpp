#include <iostream>
#include <stdexcept>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT; // input type
typedef CGAL::Gmpz ET; // exact Type
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main()
{
	std::cin.sync_with_stdio(false);

	int n;
	for (std::cin >> n; n > 0; std::cin >> n) {
		int d;
		std::cin >> d;
		Program lp(CGAL::SMALLER, false, 0, false, 0);
		for (int i = 0; i < n; ++i) {
			double norm2 = 0;
			for (int j = 0; j < d; ++j) {
				IT ai;
				std::cin >> ai;
				norm2 += ai*ai;
				lp.set_a(j, i, ai);
			}

			// check that the norm is indeed an integer
			double norm = std::floor(std::sqrt(norm2));
			if (norm2 != norm*norm)
				throw std::domain_error("Warning: norm2 != norm*norm.\n");

			lp.set_a(d, i, norm);
			IT bi;
			std::cin >> bi;
			lp.set_b(i, bi);
		}
	
		lp.set_c(d, -1);
		lp.set_l(d, true, 0);

		Solution s = CGAL::solve_linear_program(lp, ET());
		if (s.is_infeasible())
	 		std::cout << "none\n";
	 	else if (s.is_unbounded())
	 		std::cout << "inf\n";
	 	else
	 		std::cout << -(s.objective_value().numerator() / s.objective_value().denominator()) << "\n";
	}
	
	return 0;
 }