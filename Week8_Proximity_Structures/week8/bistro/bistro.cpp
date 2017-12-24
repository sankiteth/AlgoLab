#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

void testcase(int n){
	std::vector<IK::Point_2> restaurants(n);

	for(int i=0; i<n; i++){
    	std::cin >> restaurants[i];
	}

	int m;
	std::cin >> m;
	std::vector<IK::Point_2> new_restaurants(m);

	for(int i=0; i<m; i++){
		std::cin >> new_restaurants[i];
	}

	Triangulation t;
	t.insert(restaurants.begin(), restaurants.end());

	for(int i=0; i<m; i++){
		Vertex_handle v = t.nearest_vertex(new_restaurants[i]); // O(log n) operation
		IK::FT dist = CGAL::squared_distance(v->point(), new_restaurants[i]);
	
		std::cout << std::fixed << std::setprecision(0) << dist << std::endl;	
	}
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
