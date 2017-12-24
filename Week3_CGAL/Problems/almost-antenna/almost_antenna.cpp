#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

  // typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  K::Point_2 P;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  Min_circle::Support_point_iterator IT;
typedef long LLI; // long long gives error.

  LLI ceil_to_double(const K::FT& x)
  {
    double a = ceil(CGAL::to_double(x));

    // make sure by actual comparison
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;

    return (LLI)a;
    
  }

  void approach_1(int n)
  {
    P points[n];
    for (int i = 0; i < n; ++i)
    {
      LLI x, y;
      cin >> x >> y;
      points[i] = P(x,y);
    }

    K::FT best;
    for (int i = 0; i < n; ++i)
    {
      swap(points[0], points[i]);
      Min_circle mc(points+1, points+n, true);
      Traits::Circle c = mc.circle();

      if (i == 0)
      {
        best = c.squared_radius();
      }
      else
      {
        if (c.squared_radius() < best )
        {
          best = c.squared_radius();
        }
      }
    }

    cout << ceil_to_double(sqrt(best)) << endl;
  }

  void approach_2(int n)
  {
    P points[n];
    for (int i = 0; i < n; ++i)
    {
      LLI x, y;
      cin >> x >> y;
      points[i] = P(x,y);
    }

    Min_circle mc(points, points+n, true);
    Traits::Circle c = mc.circle();

    multiset<P> sup_points;
    for (IT it = mc.support_points_begin(); it != mc.support_points_end(); ++it)
    {
      sup_points.insert(*it);
    }

    K::FT best = c.squared_radius();
    for (int i = 0; i < n; ++i)
    {
      if(sup_points.count(points[i]))
      {
        swap(points[0], points[i]);
        Min_circle mc(points+1, points+n, true);
        c = mc.circle();
        if (c.squared_radius() < best )
        {
          best = c.squared_radius();
        }
      }
    }

    cout << ceil_to_double(sqrt(best)) << endl;
  }

  int main()
  {
    int n; cin >> n;
    while(n > 0)
    {
    	approach_2(n);
    	cin >> n;
    }

    return 0;
  }