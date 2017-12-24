#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Triangulation_vertex_base_with_info_2<IK::FT,IK> vertex_t; // Info parameter: IK::FT
typedef CGAL::Triangulation_face_base_2<IK> face_t;
typedef CGAL::Triangulation_data_structure_2<vertex_t,face_t> triangulation_t;
typedef CGAL::Delaunay_triangulation_2<IK,triangulation_t> delaunay_t;

double ceil_to_double(EK::FT const & x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

EK::FT S_to_hrs(EK::FT const & S){
	return CGAL::sqrt((CGAL::sqrt(S) - 1) / 2);
}

void test_case(int n){
	IK::FT l, b, r, t;
	std::cin >> l >> b >> r >> t;

	std::vector<IK::Point_2> germs(n);
	for (std::size_t i = 0; i < n; ++i)
	{
		std::cin >> germs[i];
	}

	delaunay_t trg;
	trg.insert(germs.begin(), germs.end());

	for (auto v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v)
	{
		IK::Point_2 const & g = v->point();
		IK::FT & info_g = v->info();
		IK::FT min_horizontal = std::min(g.x() - l, r - g.x());
		IK::FT min_vertical = std::min(g.y() - b, t - g.y());
		IK::FT min_overall = std::min(min_horizontal, min_vertical);
		info_g = 4 * min_overall * min_overall;
	}

	for (auto e = trg.finite_edges_begin(); e != trg.finite_edges_end(); ++e)
	{
		auto vi = e->first->vertex(trg.cw(e->second));
		IK::Point_2 const & gi = vi->point();
		IK::FT & info_gi = vi->info();
		 
		auto vj = e->first->vertex(trg.ccw(e->second));
		IK::Point_2 const & gj = vj->point();
		IK::FT & info_gj = vj->info();
    	
		IK::FT squared_distance = CGAL::squared_distance(gi, gj);
		info_gi = std::min(info_gi, squared_distance);
		info_gj = std::min(info_gj, squared_distance);
	}

	std::vector<IK::FT> min_S;
	min_S.reserve(n);
	for (auto v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v)
	{
		min_S.push_back(v->info());
	}

	std::sort(min_S.begin(), min_S.end());
	EK::FT first = S_to_hrs(EK::FT(min_S[0]));
	EK::FT median = S_to_hrs(EK::FT(min_S[n/2]));
	EK::FT last = S_to_hrs(EK::FT(min_S[n-1]));

	std::cout
	<< ceil_to_double(first) << " "
	<< ceil_to_double(median) << " "
	<< ceil_to_double(last) << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int n;
	while (std::cin >> n && n != 0){
		test_case(n);
	}
}