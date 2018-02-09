#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <algorithm>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;


// O( logn * nlogn)
void testcase(){
	int m, n;
	std::cin >> m >> n;
	std::vector<IK::Point_2> participants(m);
	std::vector<IK::FT> radius_p(m);
	for(int i=0; i<m; i++){
		std::cin >> participants[i] >> radius_p[i];
	}

	IK::FT h;
	std::cin >> h;

	std::vector<IK::Point_2> lamps(n);
	for(int i=0; i<n; i++){
		int x, y;
		std::cin >> x >> y;
		lamps[i] = IK::Point_2(x, y);
	}

	int low = 0, high = n-1;
	int sol = -1; // smallest index where some participants survive
	bool some_survived = false;
	while(low <= high){

		int mid = low + (high-low)/2;

		Triangulation t;
	  	t.insert(lamps.begin(), lamps.begin() + mid + 1); // need to add the 1 otherwise mid point excluded!!

	  	some_survived = false;

	  	for(int i=0; i<m; i++){
	  		Vertex_handle v = t.nearest_vertex(participants[i]);
	  		IK::FT dist_1 = CGAL::squared_distance(v->point(), participants[i]);
	  		IK::FT dist_2 = (radius_p[i] + h)*(radius_p[i] + h);
	  		if(dist_1 >= dist_2){ // the current participant is not trapped by any lamp in the triangulation
	  			some_survived = true;
	  			break;
	  		}
	  	}

	  	if(some_survived){ // some participants are not trapped by the lamps in the current triangulation
	  		low = mid+1;
	  		sol = mid;
	  	} else {           // all participants are trapped by the lamps in the current triangulation
	  		high = mid-1;
	  	}
	}

	if(sol == -1 || sol == 0){ // the very first lamp traps all participants, hence all survive equal time, so all winner
		for(int i=0; i<m; i++){
			std::cout << i << " ";
		}
	}else{
		Triangulation t;
		t.insert(lamps.begin(), lamps.begin()+sol+1);

	  	for(int i=0; i<m; i++){
	  		Vertex_handle v = t.nearest_vertex(participants[i]);
	  		IK::FT dist_1 = CGAL::squared_distance(v->point(), participants[i]);
	  		IK::FT dist_2 = (radius_p[i] + h)*(radius_p[i] + h);
	  		if(dist_1 >= dist_2){ // the current participant is not trapped by any lamp in the triangulation, hence will win
	  			std::cout << i << " ";
	  		}
	  	}
	}

	std::cout << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}