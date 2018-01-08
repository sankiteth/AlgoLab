#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>

#define MINUS_INF numeric_limits<int>::min()
#define INF numeric_limits<int>::max()

using namespace std;

void testcase(){
	int n, m, k;
	cin >> n >> m >> k;
	vector<int> v(n+1);

	for(int i=1; i<=n; i++){
		cin >> v[i];
	}

	vector<int> ps(n+1, 0);
	for(int i=1; i<=n; i++){
		ps[i] = ps[i-1] + v[i];
	}

	vector<int> sum_ind(n+1, -1);
	for(int i=1; i<=n; i++){

		int low = 1, high = i+1;
		int mid;
		while(low < high){ // Binary search for partition whose sum is k
			mid = low + (high - low)/2;
			int sum = ps[i] - ps[mid] + v[mid];
			if(sum == k){
				break;
			}
			else if(sum > k){
				low = mid+1;
			}
			else{
				high = mid;
			}
		}

		if(low < high){
			sum_ind[i] = mid;
		}
	}

	vector<vector<int> > dp(n+1, vector<int>(m+1, MINUS_INF));

	// Base case: 0 attackers
	for(int i=0; i<=n; i++){
		dp[i][0] = 0;
	}

	for(int i=1; i<=n; i++){ // defenders
		for(int j=1; j<=m; j++){ // attackers
			dp[i][j] = max(dp[i][j], dp[i-1][j]); // ith defender not used
			if(sum_ind[i] != -1){
				dp[i][j] = max(dp[i][j], dp[sum_ind[i]-1][j-1] + (i - sum_ind[i] + 1) ); // the partition with i used
				dp[i][j] = max(dp[i][j], dp[sum_ind[i]-1][j]); // the partition with i not used
			}
		}
	}

	if(dp[n][m] > 0){
		cout << dp[n][m] << endl;
	}
	else{
		cout << "fail" << endl;
	}
}

int main(){
	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}