#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <cmath>
#include <limits>

typedef long long LLI;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

LLI floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));

  // make sure by actual comparison
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;

  return (LLI)a;
  
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int n; std::cin >> n;

	while(n > 0)
	{
		LLI x, y, a, b;
		std::cin >> x >> y >> a >> b;
		P source(x, y);
		P direction(a, b);
		R ray = R(source, direction);

		bool does_intersect = false;
		P min_p;
		K::FT dist_so_far;

		int i;
		for (i = 0; i < n; ++i)
		{
			LLI r, s, t, u;
			std::cin >> r >> s >> t >> u;
			P p1(r, s);
			P p2(t, u);

			S seg = S(p1, p2);

			if (CGAL::do_intersect(ray, seg) )
			{
				auto o = CGAL::intersection(ray, seg);

        		if (const P* op = boost::get<P>(&*o))
	  			{
	  				// For the first time
	                if (!does_intersect)
	                {
	                	does_intersect = true;
	                	dist_so_far = CGAL::squared_distance(source, *op);
	                	min_p = *op;
	                }

	                else if (CGAL::squared_distance(source, *op) < dist_so_far )
	                {
	                	dist_so_far = CGAL::squared_distance(source, *op);
	                	min_p = *op;
	                }
	  			}
                else if (const S* os = boost::get<S>(&*o))
                {
                	K::FT dist_p1 = CGAL::squared_distance(source, p1);
					K::FT dist_p2 = CGAL::squared_distance(source, p2);

					K::FT dist = ((dist_p1 <= dist_p2) ? dist_p1 : dist_p2);

					// For the first time
	                if(!does_intersect)
	                {
						does_intersect = true;
						dist_so_far = dist_p1;
						min_p = p1;

						if(dist_p2 < dist_so_far)
						{
							dist_so_far = dist_p2;
							min_p = p2;
						}

					}
					else
					{
						if(dist_p1 < dist_so_far)
						{
							dist_so_far = dist_p1;
							min_p = p1;
						}

						if(dist_p2 < dist_so_far)
						{
							dist_so_far = dist_p2;
							min_p = p2;
						}
					}
                }        
			}
		}

		if (does_intersect)
		{
			std::cout << floor_to_double(min_p.x()) << " " << floor_to_double(min_p.y()) << std::endl;
		}
		else
		{
			std::cout << "no" << std::endl;
		}
		
		std::cin >> n;
	}
}