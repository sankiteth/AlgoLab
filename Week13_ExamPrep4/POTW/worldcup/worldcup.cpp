#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <map>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define var(w, s, num_wh, num_stad) w*num_stad+s

/*inline int var(int w, int s, int num_stad){ 
  return w*num_stad+s;
}*/

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;

typedef vector<vector<long> > vvec;
typedef vector<long> vec;

// choose exact integral type
typedef CGAL::Gmpq ET;

typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round up to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int num_wh, num_stad, num_cont;
void testcase() {
  cin >> num_wh >> num_stad >> num_cont;

  Program lp(CGAL::SMALLER, true, 0, false, 0);

  vector<long> alcohol(num_wh);
  vector<long> supply(num_wh);
  vector<Point> pts;
  vector<Point> warehouses;
  for (int i = 0; i < num_wh; i++) {
    long x,y,s,a; 
    cin >> x >> y >> s >> a;
    alcohol[i] = a;
    supply[i] = s;
    Point p = Point(x,y);
    warehouses.push_back(p);
    pts.push_back(p);
  }
  

  vector<long> demand(num_stad);
  vector<long> upper_alcohol(num_stad);
  vector<Point> stadiums;
  for (int i = 0; i < num_stad; i++) {
    long x,y,d,u; 
    cin >> x >> y >> d>> u;
    upper_alcohol[i] = u;
    demand[i] = d;
    Point p = Point(x,y);
    stadiums.push_back(p);
    pts.push_back(p);
  }

  Triangulation t;
  t.insert(pts.begin(), pts.end());

  vector<vector<double> > rev(num_wh, vector<double>(num_stad));
  for (int i = 0; i < num_wh; i++) {
    for (int j = 0; j < num_stad; j++) {
      long r;
      cin >> r;
      rev[i][j] = r;
    }
  }

  vector<pair<Point, int> > contours;
  for (int i = 0; i < num_cont; i++) {
    long x,y,r; 
    cin >> x >> y >> r;
    Point p = Point(x,y);
    Point closest = t.nearest_vertex(p)->point();

    if (CGAL::squared_distance(closest, p) < r * r) { // reject contours that do not contain either a warehouse or a stadium
      contours.push_back(make_pair(p, r));
    }
  }

  for (int s = 0; s < num_stad; s++) { // one constraint per stadium
    lp.set_b(s, demand[s]); // demand
    lp.set_r(s, CGAL::EQUAL); // demand
    lp.set_b(s + num_stad, 100 * upper_alcohol[s]); // alcohol

    for (int w = 0; w < num_wh; w++) {
      lp.set_a( var(w, s, num_wh, num_stad), s, 1 ); // demand
      lp.set_a( var(w, s, num_wh, num_stad), s + num_stad, alcohol[w] ); // alcohol
    }
  }

  for (int w = 0; w < num_wh; w++) {
    lp.set_b(2*num_stad + w, supply[w]); // supply
    for (int s = 0; s < num_stad; s++) {
      lp.set_a( var(w, s, num_wh, num_stad), 2*num_stad + w, 1 ); // supply

      double subtract = 0;

      for (int k = 0; k < contours.size(); k++) {
        Point p = contours[k].first;
        double r = contours[k].second;
        bool stadiumIsInside = CGAL::squared_distance(stadiums[s], p) < r * r;
        bool warehouseIsInside = CGAL::squared_distance(warehouses[w], p) < r * r;
        if (stadiumIsInside != warehouseIsInside) { // if the contour k is crossed
          subtract += 1;
        }
      }
      lp.set_c( var(w, s, num_wh, num_stad), -(rev[w][s]*100 - subtract));
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) {
    cout << "RIOT!" << endl;
  } else {
    cout << (long)floor_to_double(-s.objective_value()/100) << endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >>t;
  while(t--) testcase();
}