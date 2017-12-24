#include <iostream>

using namespace std;

void approach_1()
{
	int n; cin >> n;

	int mat[n][n];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cin >> mat[i][j];
		}
	}

	int partial_sum[n][n];

	partial_sum[0][0] = mat[0][0];

	//first row
	for (int j = 1; j < n; ++j)
	{
		partial_sum[0][j] = partial_sum[0][j-1] + mat[0][j];
	}

	//first col
	for (int i = 1; i < n; ++i)
	{
		partial_sum[i][0] = partial_sum[i-1][0] + mat[i][0];
	}

	//whole matrix
	for (int i = 1; i < n; ++i)
	{
		for (int j = 1; j < n; ++j)
		{
			partial_sum[i][j] = (partial_sum[i-1][j] + partial_sum[i][j-1] - partial_sum[i-1][j-1]) + mat[i][j];
		}
	}

	int even_quad = 0;

	for (int i1 = 0; i1 < n; ++i1)
	{
		for (int i2 = i1; i2 < n; ++i2)
		{
			for (int j1 = 0; j1 < n; ++j1)
			{
				for (int j2 = j1; j2 < n; ++j2)
				{
					int sum = 0;

					if (i1==0 && j1==0)
					{
						sum = partial_sum[i2][j2];
					}
					else if (i1==0)
					{
						sum = partial_sum[i2][j2] - partial_sum[i2][j1-1];
					}
					else if (j1==0)
					{
						sum = partial_sum[i2][j2] - partial_sum[i1-1][j2];
					}
					else
					{
						sum = partial_sum[i2][j2] - (partial_sum[i1-1][j2] + partial_sum[i2][j1-1] - partial_sum[i1-1][j1-1]);
					}

					if (sum%2 == 0)
					{
						even_quad++;
					}
				}
			}
		}
	}

	cout << even_quad << endl;
}

void approach_2()
{
	int n; cin >> n;

	int M[n+1][n+1];

	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			cin >> M[i][j];
		}
	}

	// pM[i][j] = sum of elements in the submatrix (1, 1, i, j).
	int pM[n+1][n+1];

	// first row and column
	for (int i = 0; i <= n; ++i)
	{
		pM[0][i] = 0;
		pM[i][0] = 0;
	}

	//whole matrix
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			pM[i][j] = (pM[i-1][j] + pM[i][j-1] - pM[i-1][j-1]) + M[i][j];
		}
	}

	int solution = 0;
	for (int i1 = 1; i1 <= n; ++i1)
	{
		for (int i2 = i1; i2 <= n; ++i2)
		{
			// Reduce the problem to one dimension
			int S[n+1]; // Do even pairs on S
			int pS[n+1]; // contains partial sums of S

			pS[0] = 0;
			for (int k = 1; k <= n; ++k)
			{
				// Fix rows i1, i2, find sum of elements in kth col. 
				S[k] = pM[i2][k] - pM[i2][k-1] - pM[i1-1][k] + pM[i1-1][k-1];
				pS[k] = pS[k-1] + S[k];
			}

			// Even pairs O(n) algorithm on S
			int onedim_sol = 0;
			int even = 0; // num of partial sums that are even
			int odd = 0; // num of partial sums that are odd

			for (int j = 1; j <= n; ++j)
			{
				if (pS[j] % 2 == 0)
				{
					// If partial sum even, solution incremented by 1. 
					// Also for each even previous partial sum, solution incremented by 1.
					onedim_sol += even + 1; 
					even++;
				}
				else
				{
					// Only for each even previous partial sum, solution incremented by 1.
					onedim_sol += odd;
					odd++;
				}
			}

			solution += onedim_sol;
		}
	}

	cout << solution << endl;
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		approach_2();
	}

	return 0;
}