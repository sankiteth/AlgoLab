#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <vector>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef K::Point_2 P;
typedef std::vector<P> PV;
typedef std::vector<std::pair<P, int> > AV;
typedef std::vector<std::pair<P, double> > SV;

void testcase(const AV& ast, SV& shoot, const PV& bh, int upperbound)
{
	int a = ast.size();
	int s = shoot.size();
	int b = bh.size();

	// compute DT
	Delaunay d;
	d.insert(bh.begin(), bh.end());

	Program lp(CGAL::LARGER, true, 0, false, 0);

	// compute max_sqdist for each shooting point
	// and set c for the LP
	for (int i = 0; i < s; ++i) {
		if (b > 0) {
			Delaunay::Vertex_handle va = d.nearest_vertex(shoot[i].first);
			shoot[i].second = CGAL::squared_distance(shoot[i].first, va->point());
		}
		lp.set_c(i, 1);
	}

	// setup the LP constraints - one for each asteroid
	for (int i = 0; i < a; ++i) {
		for (int j = 0; j < s; ++j) {
			K::FT sqdist = CGAL::squared_distance(ast[i].first, shoot[j].first);
			if (sqdist < shoot[j].second) {
				if (sqdist > 0)
					lp.set_a(j, i, 1 / ET(sqdist));
				else
					lp.set_a(j, i, 1);
			}
		}
		lp.set_b(i, ast[i].second);
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible() || sol.objective_value() > upperbound)
		std::cout << "n\n";
	else
		std::cout << "y\n";
}

int main()
{
	std::cin.sync_with_stdio(false);
	int t;
	for (std::cin >> t; t > 0; --t) {
		int a, s, b, up;
		std::cin >> a >> s >> b >> up;
		AV ast;
		for (int i = 0; i < a; ++i) {
			P as;
			int intens;
			std::cin >> as >> intens;
			ast.push_back(std::make_pair(as, intens));
		}
	
		SV shoot;
		for (int i = 0; i < s; ++i) {
			P sh;
			std::cin >> sh;
			shoot.push_back(std::make_pair(sh, 1<<30));
		}

		PV bh;
		for (int i = 0; i < b; ++i) {
		int x, y;
		std::cin >> x >> y;
		bh.push_back(P(x,y));
		}

		testcase(ast, shoot, bh, up);
	}

	return 0;
}