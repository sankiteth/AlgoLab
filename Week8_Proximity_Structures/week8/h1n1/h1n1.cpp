#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include <utility>
#include <map>
#include <unordered_map>
#include <queue>

#define INF std::numeric_limits<IK::FT>::max()

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Triangulation_vertex_base_2<IK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long, IK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK,Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::All_faces_iterator  Face_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

bool dfs(Triangulation& t, Face_iterator& f, double limit_dist, std::unordered_map<long, std::pair<int, bool> >& dfs_info){
	int face_num = f->info();
	if(t.is_infinite(f)){ // base case
		dfs_info[face_num] = std::make_pair(2, true); // make face black
		return true;
	}

	if( dfs_info.find(face_num) != dfs_info.end() && dfs_info[face_num].first == 2 ){ // face already black; return already evaluated value
		return dfs_info[face_num].second;
	}

	dfs_info[face_num] = std::make_pair(1, false);

	bool ret = false;
	for(int i=0; i<3; i++){ // for every edge of the face, check if it violates the constraint
		Vertex_handle v1 = f->vertex((i+1)%3);         // e.g, 0th vertex 
        Vertex_handle v2 = f->vertex((i+2)%3);   // e.g, 1st vertex

        IK::FT dist = squared_distance( v1->point(), v2->point() )/4;
        if(dist >= limit_dist){ // This edge permits passing through
        	Face_iterator neighbor_face = f->neighbor(i);
        	if( dfs_info.find( neighbor_face->info() ) == dfs_info.end() ){ // if the neighbor face is white, visit it
        		ret = ret || dfs(t, neighbor_face, limit_dist, dfs_info);
        	}
        }
	}

	dfs_info[face_num].first = 2;
	dfs_info[face_num].second = ret;
	return ret;
}

// O(mn)
void testcase(int n){
	std::vector<IK::Point_2> infected(n);

	for(int i=0; i<n; i++){
		std::cin >> infected[i];
	}

	int m;
	std::cin >> m;
	std::vector<IK::Point_2> escape(m);
	std::vector<IK::FT> d(m);

	for(int i=0; i<m; i++){
		std::cin >> escape[i] >> d[i];
	}

	Triangulation t;
	t.insert(infected.begin(), infected.end());

	long counter = 0;
	for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){ // number the faces
    	f->info() = counter++;
	}

	std::unordered_map<long, std::pair<int, bool> > dfs_info;
	for(int i=0; i<m; i++){
    	dfs_info = std::unordered_map<long, std::pair<int, bool> >();

		Vertex_handle v = t.nearest_vertex(escape[i]); // O(log n) operation
		IK::FT dist = CGAL::squared_distance(v->point(), escape[i]);
		if(dist < d[i]){
			std::cout << "n";
		} else {
			Face_iterator f = t.locate(escape[i]);
			
			if( dfs(t, f, d[i], dfs_info) ){
				std::cout << "y";
			} else {
				std::cout << "n";
			}
		}
	}

	std::cout << std::endl;
}

struct comparator_max {
	 bool operator()(std::pair<long, IK::FT> i, std::pair<long, IK::FT> j) {
	 return i.second <= j.second;
	 }
};

void dijikstra(std::vector< std::vector< std::pair< long, IK::FT> > >& graph, long source, std::vector< IK::FT >& radius){
	// initialize single source
	for(long i=0; i<radius.size(); i++){
		radius[i] = 0;
	}

	radius[source] = INF;

	// initialize priority queue
	std::priority_queue<std::pair<long, IK::FT>, std::vector<std::pair<long, IK::FT> >, comparator_max> Q;
	Q.push(std::make_pair(source, radius[source]) );

	while(!Q.empty()){
		std::pair<long, IK::FT> top = Q.top();
		Q.pop();
		long cur_node = top.first;
		for(long i=0; i<graph[cur_node].size(); i++){
			long neighbor = graph[cur_node][i].first;

			IK::FT update = std::max(radius[neighbor], std::min(top.second, graph[cur_node][i].second) );

			if(update > radius[neighbor]){
				radius[neighbor] = update;
				Q.push(std::make_pair(neighbor, radius[neighbor])); // enqueue only in case of better result
			}
		}
	}
}

void testcase_improved(int n){
	std::vector<IK::Point_2> infected(n);

	for(int i=0; i<n; i++){
		std::cin >> infected[i];
	}

	int m;
	std::cin >> m;
	std::vector<IK::Point_2> escape(m);
	std::vector<IK::FT> d(m);

	for(int i=0; i<m; i++){
		std::cin >> escape[i] >> d[i];
	}

	Triangulation t;
	t.insert(infected.begin(), infected.end());	

	std::vector< std::vector< std::pair< long, IK::FT> > > graph; // the adjacency list
	graph.push_back(std::vector< std::pair< long, IK::FT> >());

	long index = 1;
	for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){ // numbering the trianlges. 0 is for the super source
		f->info() = index++;
		//std::cout << f->info() << " " << t.triangle(f) << std::endl;
		graph.push_back(std::vector< std::pair< long, IK::FT> >());
	}	

	std::vector< IK::FT > radius(index); // will store max escape radius for each face of the triangulation

	// build graph for dijikstra
	for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
		long u = f->info();

		if(!t.is_infinite(f)) { // finite face
	    	for(int i=0; i<3; i++){ // for every edge of the face
				Vertex_handle v1 = f->vertex((i+1)%3);
		        Vertex_handle v2 = f->vertex((i+2)%3);

		        IK::FT dist = squared_distance( v1->point(), v2->point() )/4;
		        
		    	Face_iterator neighbor_face = f->neighbor(i);
		    	long v = neighbor_face->info();

		    	graph[u].push_back(std::make_pair(v, dist));
			}
		} else {
			graph[0].push_back(std::make_pair(u, INF)); // edge from super source to infinite faces

			for(int i=0; i<3; i++){
				if(!t.is_infinite(f, i)){ // finite edge of the infinite face
					Vertex_handle v1 = f->vertex((i+1)%3);
			        Vertex_handle v2 = f->vertex((i+2)%3);

			        IK::FT dist = squared_distance( v1->point(), v2->point() )/4;
			        
			    	Face_iterator neighbor_face = f->neighbor(i);
			    	long v = neighbor_face->info();

			    	graph[u].push_back(std::make_pair(v, dist));
				}
			}
		}
	}

	dijikstra(graph, 0, radius);

	for(int i=0; i<m; i++){
		Vertex_handle v = t.nearest_vertex(escape[i]);
		IK::FT dist = CGAL::squared_distance(v->point(), escape[i]);
		if(dist < d[i]){
			std::cout << "n";
		} else {
			Face_iterator f = t.locate(escape[i]);
			if(radius[f->info()] < d[i]){
				std::cout << "n";
			} else {
				std::cout << "y";
			}
		}
	}

	std::cout << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);

	int n;
	std::cin >> n;
	while(n!=0){
		testcase_improved(n);
		std::cin >> n;
	}

	return 0;
}