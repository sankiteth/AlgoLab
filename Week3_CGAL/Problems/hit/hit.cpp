#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef long long int LLI;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

int main()
{
	int n; std::cin >> n;

	while(n > 0)
	{
		LLI x, y, a, b;
		std::cin >> x >> y >> a >> b;
		R ray = R( P(x, y), P(a, b) );

		bool does_intersect = false;

		int i;
		for (i = 0; i < n; ++i)
		{
			LLI r, s, t, u;
			std::cin >> r >> s >> t >> u;
			S seg = S( P(r, s), P(t, u) );

			if (CGAL::do_intersect(ray, seg) )
			{
				does_intersect = true;
				break;
			}
		}

		if (does_intersect)
		{
			std::cout << "yes" << std::endl;
		}
		else
		{
			std::cout << "no" << std::endl;
		}
		
		// Read out the possible left segments
		while(++i < n)
		{
			LLI r, s, t, u;
			std::cin >> r >> s >> t >> u;
		}
		
		std::cin >> n;
	}
}