#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <cmath>

using namespace std;

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  K::Point_2 P;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef long LLI; // long long gives error.

LLI ceil_to_double(const K::FT& x)
{
  double a = ceil(CGAL::to_double(x));

  // make sure by actual comparison
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;

  return (LLI)a;
  
}

int main()
{
  int n; cin >> n;
  while(n > 0)
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
  	cout << ceil_to_double(sqrt(c.squared_radius()) ) << endl;

  	cin >> n;
  }

  return 0;
}