#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef std::vector<P> Pts;
typedef std::vector<std::size_t> Covered;
typedef std::vector<Covered> Trs;
typedef Covered::const_iterator CCI;

inline bool contains(const Pts& t, const P& p) 
{
	return !CGAL::right_turn(t[0],t[1],p) &&
			!CGAL::right_turn(t[2],t[3],p) &&
			!CGAL::right_turn(t[4],t[5],p);
}

void find_cover()
{
	// read input
	std::size_t m, n;
	std::cin >> m >> n;

	Pts path;
	path.reserve(m); // No initialization. Only memory reserved.
	for (std::size_t i = 0; i < m; ++i) 
	{
		P p;
		std::cin >> p;
		path.push_back(p);
	}

	Trs triangles(n);
	for (std::size_t i = 0; i < n; ++i) 
	{
		Pts t;
		for (std::size_t j = 0; j < 6; ++j) 
		{
			P p;
			std::cin >> p;
			t.push_back(p);
		}

		// ensure correct order for orientation tests
		for (std::size_t j = 0; j < 6; j+=2)
		{
			if (CGAL::right_turn(t[j],t[j+1],t[(j+2)%6])) 
			{
				std::swap(t[j],t[j+1]);
			}
		}

		// store which path segments are covered
		bool prev = contains(t,path[0]);
		for (std::size_t j = 1; j < m; ++j)
		{
			if (contains(t,path[j])) 
			{
				if (prev) 
					triangles[i].push_back(j-1); //Path j-1 in triangle i
				else 
					prev = true;
			} 
			else
			{
				prev = false;
			}
		}
	}

	// search for the cover by scanning through the sequence of triangles
	Covered covered(m-1,0); // #times i,i+1 is covered
	std::size_t uncovered = m-1; // #uncovered segments (covered[i]==0)
	std::size_t best = n; // size of best range so far
	for (std::size_t tb = 0, te = 0; tb != triangles.size(); ) 
	{
		// ensure covering
		for ( ; uncovered > 0 && te != triangles.size(); ++te)
		{
			for (CCI j = triangles[te].begin(); j != triangles[te].end(); ++j)
				if (++covered[*j] == 1) 
					--uncovered;
		}

		if (uncovered != 0) 
				break;

		// can we remove tb?
		do
			for (CCI j = triangles[tb].begin(); j != triangles[tb].end(); ++j)
				if (--covered[*j] == 0) 
					++uncovered;
		while (++tb != te && uncovered == 0);

		best = std::min(best,te-tb+1);
	}

	std::cout << best << "\n";
}	

int main(){
	std::ios_base::sync_with_stdio(false);

	int c; std::cin >> c;
	for (int i = 0; i < c; ++i)
	{
		find_cover();
	}

	return 0;
}