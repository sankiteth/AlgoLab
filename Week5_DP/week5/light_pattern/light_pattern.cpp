#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
#include <utility>

using namespace std;

int n,k,x;
vector<int> lights;
vector<int> diffs_within_group;
vector<vector<int> > memo;

int f(int g, bool prev, int level)
{
	//cout << string(level, ' ') << "current " << g << " bool= " << prev << endl;
	if (memo[prev][g] != -1)
	{
		/*cout << string(level, ' ') << "current " << g << " bool= " << prev << " ret1: " << memo[make_pair(g, prev)] << endl;
		cout << endl;*/
		return memo[prev][g];
	}

	int ops_without_group_flip = diffs_within_group[g];
	int ops_with_group_flip = k - ops_without_group_flip;
	/*cout << "current " << g << " " << prev << endl;
	cout << "ops_with_flip " << ops_with_group_flip << endl;
	cout << "ops_without_flip " << ops_without_group_flip << endl;*/

	if (prev == true)
	{
		memo[1][g] = min(f(g-1, true, level+2)      + ops_with_group_flip,
						 f(g-1, false, level+2) + 2 + ops_with_group_flip // flip till g group, then again flip till (g-1) 
									); 
	}
	else
	{
		memo[0][g] = min( f(g-1, false, level+2), f(g-1, true, level+2) ) + ops_without_group_flip;
	}
	/*cout << string(level, ' ') << "current " << g << " bool= " << prev << " ret2: " << memo[make_pair(g, prev)] << endl;
	cout << endl;*/
	return memo[prev][g];
}

void light_pattern()
{
	cin >> n >> k >> x;
	lights = vector<int>(n);
	diffs_within_group = vector<int>(n/k + 1);

	for (int i = 0; i < n; ++i)
	{
		cin >> lights[i];
	}

	//pre-calculate the number of mis-matches in each group
	for (int g = 1; g <= n/k; ++g)
	{
		int diff_within_group_g = 0;
		for (int i = g*k - 1, j = 0; i >= (g-1)*k; --i, j++)
		{
			// ith buld does not match the desired patterns
			if ( lights[i] != ( (x>>j) & 1 ) ) 
			{
				diff_within_group_g++;
			}
		}

		diffs_within_group[g] = diff_within_group_g;
	}	

	memo = vector<vector<int> >(2, vector<int>(n/k + 1, -1));
	memo[0][0] = 0;
	memo[1][0] = 1;

	cout << min(f(n/k, true, 0),f(n/k,false, 0)) << endl;
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		light_pattern();
	}
	
	return 0;
}