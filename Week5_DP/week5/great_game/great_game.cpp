#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>

#define INF numeric_limits<int>::max()
#define MINUS_INF numeric_limits<int>::min()

using namespace std;

vector<vector<int> > trans;
int n,m;
int r,b;

// map of current node to (next node, best distance from current node) 
vector<pair<int,int> > good_dp;
vector<pair<int,int> > bad_dp;

void great_game()
{
	cin >> n >> m;
	cin >> r >> b;

	trans = vector<vector<int> >(n+1);
	for (int i = 0; i < m; ++i)
	{
		int u,v; cin >> u >> v;
		trans[u].push_back(v);
	}

	good_dp = vector<pair<int,int> >(n+1);
	good_dp[n] = make_pair(0, n);

	bad_dp = vector<pair<int,int> >(n+1);
	bad_dp[n] = make_pair(0, n);

	for (int i = n-1; i >= 1; --i)
	{
		int best_dist = INF;
		int best_next;
		int worst_dist = MINUS_INF;
		int worst_next;
		for (auto it = trans[i].begin(); it != trans[i].end(); ++it)
		{
			if (1 + bad_dp[*it].first < best_dist) // best move considering the worst case scenario for the next move
			{
				best_dist = 1 + bad_dp[*it].first;
				best_next = *it;
			}

			if (1 + good_dp[*it].first > worst_dist) // worst move considering the best case scenario for the next move 
			{
				worst_dist = 1 + good_dp[*it].first;
				worst_next = *it;
			}
		}

		good_dp[i] = make_pair(best_dist, best_next);
		bad_dp[i] = make_pair(worst_dist, worst_next);
	}

	bool is_sher_turn = true;
  	int sher_turn = 0;
  	int mor_turn = 0;
  
  	while(true)
  	{
  		if (r == n)
  		{
  			cout << 0 << endl;
  			break;
  		}

  		if (b == n)
  		{
  			cout << 1 << endl;
  			break;
  		}
  
  		if (is_sher_turn)
  		{
  			is_sher_turn = false;
  			if (++sher_turn % 2 == 1)
  			{
  				// Sherlock plays red
  				r = good_dp[r].second;
  			}
  			else
  			{
  				// Sherlock plays black
  				b = bad_dp[b].second;
  			}
  		}
  		else
  		{
  			is_sher_turn = true;
  			if (++mor_turn % 2 == 1)
  			{
  				// Moriarty plays black
  				b = good_dp[b].second;
  			}
  			else
  			{
  				// Moriarty plays red
  				r = bad_dp[r].second;
  			}
  		}
  	}
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		great_game();
	}
}