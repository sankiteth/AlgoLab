#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <limits>
#include <utility>

#define NEG_INF numeric_limits<int>::min()

using namespace std;

int n;
vector<int> m;
vector<vector<int> > c;
unordered_map <vector<int>, int, boost::hash<std::vector<int> > > memo;
vector<vector<vector<int> > > first_color;
int num_colors = (1<<10) + 1;

// For all the subsets of the set of chips from the topmost layer of all stacks, recurse.  
int f(vector<int> m)
{
	if (memo.find(m) != memo.end())
	{
		//cout << "found: " << memo[m] << endl;
		return memo[m];
	}

	// check subsets of the top chips
	// check all subsets of switches

	int maxi = NEG_INF;
	for (int s = 1; s < (1<<n); ++s)
	{
		vector<int> subset;
		bool flag = true;
		for (int i = 0; i < n; ++i)
		{
			if (s & (1 << i))
			{
				if (m[i] <= 0)
				{
					flag = false;
					break;
				}
				subset.push_back(i);
			}
		}
		// subset contains a stack which has no chip left
		if (!flag)
		{
			continue;
		}
		int last_color = c[subset[0]][ m[subset[0]]-1 ];
		flag = true;
		for (int i = 1; i < subset.size(); ++i)
		{
			int current_stack = subset[i];
			if (last_color != c[current_stack][ m[current_stack]-1 ])
			{
				// all chips in the subset are not of the same color
				flag = false;
				break;
			}
		}

		// subset contains chips of different colors
		if (!flag)
		{
			continue;
		}

		vector<int> new_m(m);
		for (int i = 0; i < subset.size(); ++i)
		{
			new_m[ subset[i] ] -= 1;
		}

		//cout << "subset size:" << subset.size() << endl;
		if (subset.size() > 1)
		{
			maxi = max(maxi, ( 1<<(subset.size()-2) ) + f(new_m));
		}
		else
		{
			maxi = max(maxi, f(new_m));
		}	
	}

	//cout << "returned:" << maxi << endl;
	memo[m] = maxi;
	return maxi;
}

// Consider topmost chip of stack 0, c0. Let its color be c. Either the chip is part of the solution or not. 
// When it is part of the solution, it definitely gets clubbed with one or more chips of its own color from the other stacks. 
// For the other stacks, find the chip with color c and maximum height. For a set of these. Now consider all subsets of this set,
// club the subset with chip c0, and recurse.
int g(const vector<int> heights)
{
	if (memo.find(heights) != memo.end())
	{
		return memo[heights];
	}

	int maxi = NEG_INF;
	vector<bool> stack_used(n, false);
	for (int stack = 0; stack < n; ++stack) // consider top chip on each stack
	{
		// if top chip of stack is in solution......

		if (stack_used[stack] || heights[stack] < 1) // if the topmost chip is already part of the solution,
		{											  // or the height of the stack has become zero, consider next stack
			continue;
		}

		stack_used[stack] = true;
		int chip_color = c[stack][ heights[stack]-1 ]; // color of the current chip
		vector<pair<int,int> > chip_set; // set of chips from other stacks with color 'chip_color' and max height
		for (int i = 0; i < n; ++i)
		{
			if ( (i != stack) && (heights[i] > 0) ) // consider the stack iff height greater than 0
			{
				// the index of first chip in stack i of color 'chip_color', having height less than or equal to heights[i]
				int col_ith = first_color[chip_color][i][ heights[i]-1 ];
				if (col_ith == (heights[i]-1)) // if top most chip of ith stack
				{
					stack_used[i] = true;
				}

				if (col_ith != -1) // If such chip exists in stack i
				{
					chip_set.push_back(make_pair(col_ith, i));
				}
			}
		}

		vector<int> new_heights;
		// each non-empty subset of chip_set, because top element of stack alone score 0.
		for (int s = 1; s < (1<<chip_set.size()); ++s)
		{
			new_heights = vector<int>(heights);
			int subset_size = 1; // top chip of current stack already in subset
			for (int i = 0; i < chip_set.size(); ++i)
			{
				if (s & (1 << i)) // existence in current subset
				{
					subset_size++;
					new_heights[chip_set[i].second] = chip_set[i].first; 
				}
			}

			if (subset_size > 1)
			{
				new_heights[stack] -= 1;
				maxi = max(maxi, ( (1<<(subset_size) - 2) ) + g(new_heights));
			}
		}

		// if top chip of stack not in solution....
		new_heights = vector<int>(heights);
		new_heights[stack] -= 1;
		maxi = max(maxi, g(new_heights));
	}

	memo[heights] = maxi;
	return maxi;
}

void testcase()
{
	m.clear();
	cin >> n;
	int max_height = -1;
	for (int i = 0; i < n; ++i)
	{
		int m_i; cin >> m_i;
		m.push_back(m_i);
		max_height = max(max_height, m_i);
	}

	// num of colors, stacks, height in particular stack
	// [i,j,k] gives the first index of element in jth stack with color i and height less than or 
	// equal to j  
	first_color = vector<vector<vector<int> > >(num_colors, vector<vector<int> >(n, vector<int>(max_height, -1)));
	c = vector<vector<int> >(n, vector<int>());

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m[i]; ++j)
		{
			int c_ij; cin >> c_ij;
			c[i].push_back(c_ij);
			first_color[c_ij][i][j] = j;
		}
	}

	for (int i = 1; i < num_colors; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int last_val = first_color[i][j][0];
			for (int k = 1; k < m[j]; ++k)
			{
				if (first_color[i][j][k] == -1)
				{
					first_color[i][j][k] = last_val;
				}
				else
				{
					last_val = first_color[i][j][k];
				}
			}
		}
	}

	memo = unordered_map <vector<int>, int, boost::hash<std::vector<int> > >();

	// base cases
	memo[vector<int>(n, 0)] = 0;
	for (int i = 0; i < n; ++i)
	{
		vector<int> v(n, 0);
		for (int j = 0; j <= m[i]; ++j)
		{
			v[i] = j;
			memo[v] = 0;
		}
	}

	cout << g(m) << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		testcase();
	}

	return 0;
}