#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void func()
{
	int n, m; cin >> n >> m;

	vector<pair<int, int> > jedi(n+1, pair<int,int>());

	for (int i = 1; i <= n; ++i)
	{
		int a, b; cin >> a >> b;
		jedi[i].first = a;
		jedi[i].second = b;
	}

	sort(jedi.begin()+1, jedi.end(), [] (pair<int, int> x, pair<int, int> y) -> bool
										{
											return (x.second < y.second); 
										}
		);

	// Only 1 of the wrapped around Jedis can be in the final solution, as 
	// the wrapped around Jedis overlap among themselves.

	int sol = 0;
	int farthest = -1;
	int first = -1; 
	int last = -1;

	for (int i = 1; i <= n; ++i)
	{
		// consider only unwrapped jedis
		if (jedi[i].first > jedi[i].second)
		{
			continue;
		}

		// compute best solution without considering wrapped around jedis
		if (jedi[i].first > farthest)
		{
			if (first == -1)
			{
				first = i;
			}

			last = i;
			sol++;
			farthest = jedi[i].second;
		}
	}

	// if the Jedi with earliest finish time is wrapped around, 
	// the final solution might start with the Jedi that finishes before the 
	// finish of first unwrapped Jedi, and starts after the finish of last unwrapped jedi 
	// in the above found solution.
	if (jedi[1].first > jedi[1].second)
	{
		// Find all the wrapped jedi to start with.
		vector<int> start_wrapped;
		int i = 1;
		// Only consider the wrapped Jedis that start
		// after the finish of last unwrapped Jedi
		while (jedi[i].first > jedi[i].second)
		{
			if (jedi[i].first > jedi[last].second)
			{
				start_wrapped.push_back(i);
			}

			i++;
		}

		for(vector<int>::iterator it=start_wrapped.begin(); it != start_wrapped.end(); ++it)
		{
				int wrapped_sol = 1;
				int limit = jedi[*it].first;
				farthest  = jedi[*it].second;

				for (int i = (*it) + 1; i <= n; ++i)
				{
					// consider only unwrapped jedis
					if (jedi[i].first > jedi[i].second)
					{
						continue;
					}

					// compute best solution without considering wrapped around jedis
					if (jedi[i].first > farthest && jedi[i].second < limit)
					{
						wrapped_sol++;
						farthest = jedi[i].second;
					}
				}

				sol = max(sol, wrapped_sol);
		}
	}

	cout << sol << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		func();
	}

	return 0;
}