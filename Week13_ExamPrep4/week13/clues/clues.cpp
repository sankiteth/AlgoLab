#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,IK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, IK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK,Tds> Triangulation;

typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef boost::disjoint_sets_with_storage<> Uf;

void testcase(){
	int n, m; 
	IK::FT r;
	std::cin >> n >> m >> r;

	IK::FT r_squared = r*r;

	std::vector< std::pair<IK::Point_2, int> > stations;
	for(int i=0; i<n; i++){
		IK::Point_2 p;
		std::cin >> p;
		stations.push_back(std::make_pair(p, i));
	}

	std::vector<IK::Point_2> holmes(m), watson(m);
	for(int i=0; i<m; i++){
		std::cin >> holmes[i] >> watson[i];
	}

	Triangulation trg;
	trg.insert(stations.begin(), stations.end());

	std::unordered_map <int, bool> face_intf;
	int index = 0;
	for (Face_iterator f = trg.finite_faces_begin(); f != trg.finite_faces_end(); ++f){
		f->info() = index++; // set info field of the face
		Vertex_handle v1 = f->vertex((1)%3);
        Vertex_handle v2 = f->vertex((2)%3);
        Vertex_handle v0 = f->vertex((3)%3);

        IK::FT squared_distance_12 = CGAL::squared_distance( v1->point(), v2->point() );
        IK::FT squared_distance_20 = CGAL::squared_distance( v2->point(), v0->point() );
        IK::FT squared_distance_10 = CGAL::squared_distance( v1->point(), v0->point() );
        if(squared_distance_10 <= r_squared && squared_distance_20 <= r_squared && squared_distance_12 <= r_squared){
        	face_intf[f->info()] = true;
        }
        else{
        	face_intf[f->info()] = false;	
        }
	}

	Uf group(n);
	for (Face_iterator f = trg.finite_faces_begin(); f != trg.finite_faces_end(); ++f){
		int info = f->info();
		if(face_intf[info]){
			continue;
		}

		for(int i=0; i<3; i++){ // for all neighbor faces
			Face_handle f_neighbor = f->neighbor(i);
			if(face_intf[ f_neighbor->info() ]){
				continue;
			}

			
		}
	}

	for(int i=0; i<m; i++){
		Vertex_handle a = trg.nearest_vertex(holmes[i]);
		Vertex_handle b = trg.nearest_vertex(watson[i]);
		bool holmes_in_range = (CGAL::squared_distance(holmes[i], a->point()) <= r_squared);
		bool watson_in_range = (CGAL::squared_distance(watson[i], b->point()) <= r_squared);

		if(!holmes_in_range && !watson_in_range && CGAL::squared_distance(holmes[i], watson[i]) <= r_squared){
			std::cout << "y"; // both out of range of stations but with each others range
			continue;
		}
		
		if(holmes_in_range && watson_in_range){ // both in range of radio stations
			if( group.find_set(a->info()) == group.find_set(b->info()) ){
				std::cout << "y";
				continue;
			}
		}

		if(holmes_in_range && !watson_in_range && 
			CGAL::squared_distance(holmes[i], watson[i]) <= r_squared){
			std::cout << "y"; // watson out of range of stations but within holmes's range
			continue;
		}
		
		if(watson_in_range && !holmes_in_range &&
			CGAL::squared_distance(holmes[i], watson[i]) <= r_squared){
			std::cout << "y"; // holmes out of range of stations but within watson's range
			continue;
		}

		// All other cases, no communication possible
		std::cout << "n";
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