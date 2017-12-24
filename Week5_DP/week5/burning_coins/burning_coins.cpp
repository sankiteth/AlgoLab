#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#define INF numeric_limits<int>::max()

using namespace std;

int n;
vector<vector<int>> memo;
vector<int> coins;

void burning_coins()
{
	int n; cin >> n;
	vector<int> coins(n);
	for (int i = 0; i < n; ++i)
	{
		cin >> coins[i];
	}

	vector<vector<int>> best(n, vector<int>(n, -1));

	// base cases
	for (int i = 0; i < n; ++i)
	{
		best[i][i] = coins[i];
		if (i > 0)
		{
			best[i-1][i] = max(coins[i-1], coins[i]);
		}
	}

	// sol of coin[i, i+l-1]
	for (int l = 3; l <= n; ++l)
	{
		for (int i = 0; i <= n-l; ++i)
		{
			int j = i+l-1;
			best[i][j] = max(coins[i] + min(best[i+1][j-1], best[i+2][j]),
							coins[j] + min(best[i+1][j-1], best[i][j-2])
							);
		}
	}

	cout << best[0][n-1] << endl;
}

int f(int i, int j)
{
	if (memo[i][j] != -1)
	{
		return memo[i][j];
	}

	memo[i][j] = max(coins[i] + min(f(i+1, j-1), f(i+2, j)),
					coins[j] + min(f(i+1, j-1), f(i, j-2))
					);

	return memo[i][j];
}

void burning_coins_memo()
{
	cin >> n;
	coins = vector<int>(n);
	memo = vector<vector<int> >(n, vector<int>(n, -1));
	for (int i = 0; i < n; ++i)
	{
		cin >> coins[i];
	}

	// base cases
	for (int i = 0; i < n; ++i)
	{
		memo[i][i] = coins[i];
		if (i > 0)
		{
			memo[i-1][i] = max(coins[i-1], coins[i]);
		}
	}

	cout << f(0, n-1) << endl;
}

int main()
{
	int t; cin >> t;	
	for (int i = 0; i < t; ++i)
	{
		burning_coins_memo();
	}

	return 0;
}