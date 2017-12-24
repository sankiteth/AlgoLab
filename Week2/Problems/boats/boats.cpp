#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

void boats()
{
	
	int n; cin >> n;

	vector<pair<int, int> > l_p;

	for (int i = 0; i < n; ++i)
	{
		int l, p;
		cin >> l >> p;

		l_p.push_back(make_pair(l, p));
	}

	sort(l_p.begin(), l_p.end(), 
			[] (pair<int, int> x, pair<int, int> y) -> bool
			{
				return x.second < y.second; 
			}
		);

	/*for (int i = 0; i < n; ++i)
	{
		cout << l_p[i].second << " ";
	}*/

	/*int prev1_p = l_p[0].second, prev2_p = -1;
	int prev1 = 0, prev2 = -1;*/
	
	int prev1_right = l_p[0].second;
	int prev2_right;
	int sol_till_now = 1;

	for (int i = 1; i < n; ++i)
	{
		if (prev1_right <= l_p[i].second)
		{
			sol_till_now++;
			prev2_right = prev1_right;

			prev1_right += l_p[i].first;

			if (prev1_right < l_p[i].second)
			{
				prev1_right = l_p[i].second;
			}
		}
		else //prev1_right > l_p[i].second
		{
			if (prev2_right+l_p[i].first < prev1_right)
			{
				prev1_right = prev2_right+l_p[i].first;

				if (prev1_right < l_p[i].second)
				{
					prev1_right = l_p[i].second;
				}
			}
		}
	}

	cout << sol_till_now << endl;
	
}

int main()
{
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		boats();
	}
}