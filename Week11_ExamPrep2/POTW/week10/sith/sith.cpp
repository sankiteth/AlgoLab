#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

// CGAL Part
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K>                  Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>         Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>               Triangulation;

typedef Triangulation::Vertex_handle                        VertexTr;
typedef Triangulation::Finite_vertices_iterator             VertexItTr;
typedef Triangulation::Edge_circulator                      EdgeCiTr;
typedef Triangulation::Edge_iterator                      EdgeItTr;
typedef K::Point_2 P;

// BGL Part
typedef adjacency_list<vecS, vecS, undirectedS,
		no_property,
		no_property
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		    EdgeIt;	// Edge iterator


void testcase() {
    int n; double r; cin >> n >> r;
    double rr = r*r;
    vector<P> pts(n);
    for(int i=0; i<n; i++){
        int x,y; cin >> x >> y;
        pts[i] = P(x,y);
    }

    int startel = n/2; // startel .. n-1 always able to build components with these pts
    int maxk = n/2;
    Triangulation t;
    t.insert(pts.begin()+startel, pts.end());

    // add index info to triangulation
    for(int i=startel; i<n; i++){ // add indexes to triangulation
        VertexTr v = t.nearest_vertex(pts[i]);
        assert(pts[i] == v->point());
        v->info() = i;
    }

    // Create BGL Graph based on triangulation
    Graph G(n);
    /*for(VertexItTr it = t.finite_vertices_begin(); it != t.finite_vertices_end(); it++){
        EdgeCiTr eit = t.incident_edges(it);
        do{
            if(t.is_infinite(*eit)) continue; 
            if(t.segment(*eit).squared_length() <= rr){ // belong together
                VertexTr u,v;
                u = eit->first->vertex((eit->second+1)%3);
                v = eit->first->vertex((eit->second+2)%3);
                add_edge(u->info(),v->info(),G);
            }
        } while(++eit != t.incident_edges(it));
    }*/

    for(EdgeItTr eit = t.finite_edges_begin(); eit != t.finite_edges_end(); eit++){
        if(t.segment(eit).squared_length() <= rr){ // part of the graph
            VertexTr u,v;
            u = eit->first->vertex((eit->second+1)%3);
            v = eit->first->vertex((eit->second+2)%3);
            add_edge(u->info(),v->info(),G);
        }
    }

    // Create connected components
    vector<int> componentmap(n);
	int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 
	map<int,vector<int> > components;

	for(int i=0; i<n; i++){
        int comp_id = componentmap[i];
        components[comp_id].push_back(i);
    }
    int best = 0;
	for(int i=0; i<ncc; i++) best = max(best, (int)components[i].size());
    if(best >= maxk){ cout << maxk << "\n"; return; }

    // try better solution by ending earlier, having thus more planets availabe (less are conquered)
    for(startel--; startel>=1; startel--){
        maxk = startel; // maximal achievable k now
        if(maxk == 1 || best >= maxk){ cout << maxk << "\n"; return; }

        // add new vertex
        VertexTr v = t.insert(pts[startel]);
        v->info() = startel; // id info

        // find all groups vertex belongs to
        EdgeCiTr eit = t.incident_edges(v);
        set<int> groups;
        do{
            if(t.is_infinite(*eit)) continue; 
            if(t.segment(*eit).squared_length() <= rr){ // belong together
                VertexTr v1,v2;
                v1 = eit->first->vertex((eit->second+1)%3);
                v2 = eit->first->vertex((eit->second+2)%3);
                if(v1 != v && v2 != v) assert(false);
                if(v1 == v) v1 = v2; // want v1 to be the other vertex
                int el_id = v1->info();
                int group = componentmap[el_id];
                groups.insert(group);
            }
        } while(++eit != t.incident_edges(v));
        if(groups.empty()) continue; // can't be a improvement
        // add to first group
        int first_group = *groups.begin();
        componentmap[startel] = first_group;
        components[first_group].push_back(startel);
        // merging of groups, put all into first group
        for(int i=1; i<groups.size(); i++){
            auto cur_it = groups.begin();
            std::advance(cur_it, i);
            const int cur_group = *cur_it;
            vector<int> &cur_els = components[cur_group];
            // merge all elements into new group
            components[first_group].insert(components[first_group].end(), cur_els.begin(), cur_els.end());
            // set all elements to new groups id
            for(int j=0; j<cur_els.size(); j++){
                componentmap.at(cur_els[j]) = first_group;
            }
            // remove old group
            components.erase(cur_group);
        }
        // check if new formed group is better
        if(components[first_group].size() >= maxk){ cout << maxk << "\n"; return; }
    }
}

int main() {
	ios_base::sync_with_stdio(false);
    size_t t; cin >> t;
    while(t--) testcase();

	return 0;
}
