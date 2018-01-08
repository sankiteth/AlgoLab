#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>

using namespace std;

#define MINUS_INF numeric_limits<int>::min()

void testcase(){
	int n; cin >> n;
	vector<vector<int> > a = vector<vector<int>  >(n+1, vector<int>(n+1));

	for(int i=1; i<=n; i++){
		for(int j=1; j<=n; j++){
			cin >> a[i][j];
		}
	}

	int num_rows = 2;
	long dp[num_rows][n+1][n+1] = {}; // dp [r][i][j]: Max collected till row r, with first path ending at col i, second at col j, i<j
							   // r=0 for even rows, 1 for odd rows; counting starts from 1

	// base case for first row
	//cout << "First row:\n";
	for(int i=1; i<n; i++){
		for(int j=1; j<=n; j++){
			dp[1][i][j] = dp[1][i][j-1] + a[1][j]; // first row of maze, only 2nd path collects
			//cout << dp[1][i][j] << " ";
		}
	}

	//cout << endl;

	for(int r=2; r<=n; r++){           // Each row of the maze
		long ps[n+1][n+1] = {};
		for(int i=1; i<=n; i++){
			for(int j=i; j<=n; j++){
				ps[i][j] = ps[i][j-1] + a[r][j];
			}
		}

		for(int i=1; i<n; i++){        // First path
			for(int j=i+1; j<=n; j++){ // Second path

				long max_rij = MINUS_INF;
				for(int k1=1; k1<=i; k1++){
					for(int k2=i+1; k2<=j; k2++){
						max_rij = max(max_rij, dp[(r-1)%2][k1][k2] + ps[k1][i] + ps[k2][j]);
					}
				}

				dp[r%2][i][j] = max_rij;
			}
		}
	}

	cout << dp[n%2][n-1][n] << endl;
}

int main(){
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}