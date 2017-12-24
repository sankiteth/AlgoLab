#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

double ceil_to_double(const EK::FT& x)
  {
    double a = ceil(CGAL::to_double(x));

    // make sure by actual comparison
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;

    return (double)a;
  }

void testcase(int n){
	std::vector<IK::Point_2> pts(n);

	for(int i=0; i<n; i++){
    	std::cin >> pts[i];
	}

	Triangulation t;
  	t.insert(pts.begin(), pts.end());

  	// output all edges
  	IK::FT  min_squared_dist = std::numeric_limits<IK::FT>::max();
  	int index = 0;
  	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
    	min_squared_dist = std::min(min_squared_dist,
    								t.segment(e).squared_length()
    								);
  	}

  	std::cout << ceil_to_double((CGAL::sqrt( EK::FT(min_squared_dist) )/2) * 100) << std::endl; // time in one hundredth of a second
}

int main(){
	std::ios_base::sync_with_stdio(false);

	int n;
	std::cin >> n;
	while(n!=0){
		testcase(n);
		std::cin >> n;
	}
}