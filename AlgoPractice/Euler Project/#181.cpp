#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
	int q; cin >> q;

	const unsigned int M = 10^9 + 7;

	// 0 <= n,m <= 160.
	// So, (m+n+i-1) is always <= 640
	const int MAX = 641;
	int C[MAX][MAX];

	// Fill all diagonals first
	for (int i = 0; i < MAX; ++i)
	{
		C[i][i] = 1;
	}

	for (int i = 1; i < MAX; ++i)
	{
		C[i][0] = 1; // C(i, 0) = 1
		for (int j = 1; j < i; ++j)
		{
			C[i][j] = ( C[i-1][j] + C[i-1][j-1] ) % M;
		}
	}

	// Printing out the Pascal's triangle
	/*for (int i = 0; i < MAX; ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			cout << C[i][j] << " ";
		}

		cout << endl;
	}*/

	for (int test = 0; test < q; ++test)
	{
		int n, m; cin >> n >> m;

		// WLOG, m is smaller, n is larger
		int mini = min(m, n);
		int maxi = max(m, n);

		m = mini; n = maxi;

		int groups = 0;

		for (int i = 2; i <= (m+n); ++i)
		{
			// Divide n black objects and m white objects in i groups
			// Calculated as: ( m+n+i-1 )! / ( m! * n! * (i-1)! ) = (m+n+i-1)C(i-1) * (m+n)C(m)

			int cur = ( C[m+n+i-1][i-1] * C[m+n][m] ) % M;

			cout << cur << endl;
			groups += cur;
		}

		cout << groups << endl;
	}

	return 0;
}