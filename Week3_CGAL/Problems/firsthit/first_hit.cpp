#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <cmath>
#include <limits>

typedef long long LLI;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef std::result_of<K::Intersect_2(R, S)>::type IT;

LLI floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));

  // make sure by actual comparison
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;

  return (LLI)a;
  
}

// clip/set target of s to o
void shorten_segment(K::Segment_2& s, const IT& o) {
	if (const P* p = boost::get<P>(&*o))
		s = S(s.source(), *p);
	else if (const S* t = boost::get<S>(&*o))
		// select endpoint of *t closer to s.source()
		if (CGAL::collinear_are_ordered_along_line( s.source(), t->source(), t->target() ))
			s = S(s.source(), t->source());
		else
			s = S(s.source(), t->target());
	else
		throw std::runtime_error("Strange segment intersection.");
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int n;
	while((std::cin >> n) && n > 0)
	{
		LLI x, y, a, b;
		std::cin >> x >> y >> a >> b;
		P source(x, y);
		P direction(a, b);
		R ray = R(source, direction);

		bool does_intersect = false;
		P min_p;
		K::FT dist_so_far;
		std::vector<S> segs;
		for (int i = 0; i < n; ++i)
		{
			LLI r, s, t, u;
			std::cin >> r >> s >> t >> u;
			segs.push_back(S( P(r, s), P(t, u)));
		}

		std::random_shuffle(segs.begin(), segs.end());
		S clipped_ray(ray.source(), ray.source());

		// find some segment hit by ray
		std::size_t i = 0;
		for (; i < n; ++i){
			if (CGAL::do_intersect(segs[i], ray)) {
				shorten_segment(clipped_ray, CGAL::intersection(segs[i], ray));
				break;
			}
		}

		if (i == n) { 
			std::cout << "no\n"; 
			continue;
		}

		// check remaining segments against rc
		while (++i < n){
			if (CGAL::do_intersect(segs[i], clipped_ray)){
				shorten_segment(clipped_ray, CGAL::intersection(segs[i], ray)); // not clipped_ray!
			}
		}
	
		std::cout << floor_to_double(clipped_ray.target().x()) << " " << floor_to_double(clipped_ray.target().y()) << "\n";
	}
}