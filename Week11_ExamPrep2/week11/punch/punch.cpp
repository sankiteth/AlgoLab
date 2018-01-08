#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>
#include <iomanip>

using namespace std;

typedef long LLI;

#define INF numeric_limits<LLI>::max()
#define MINUS_INF numeric_limits<int>::min()

int maxDrinks(int c[], int m, int V)
{
    // table[i] will be storing the maximum number of drinks
    // required for i value.  So table[V] will have result
    int table[V+1];
 
    // Base case (If given value V is 0)
    table[0] = 0;
 
    // Initialize all table values as Infinite
    for (int i=1; i<=V; i++)
        table[i] = INT_MIN;
 
    // Compute maximum number of drinks required for all
    // values from 1 to V
    for (int i=1; i<=V; i++)
    {
        // Go through all drinks with cost smaller than i
        for (int j=1; j<=m; j++)
          if (c[j] <= i)
          {
              int sub_res = table[i-c[j]];
              if (sub_res != INT_MIN && sub_res + 1 > table[i])
                  table[i] = sub_res + 1;
          }
    }
    return table[V];
}

void testcase(){
	int n, k;
	cin >> n >> k;

	int c[n+1], v[n+1];
	for(int i=1; i<=n; i++){
		cin >> c[i] >> v[i];
	}

	vector<vector<LLI> > dp(n+1, vector<LLI>(2*k+1, INF) );
	vector<vector<int> > drink_used(n+1, vector<int>(2*k+1, 0) );
	vector<vector<int> > num_drinks(n+1, vector<int>(2*k+1, MINUS_INF) );


	for(int j=1; j<=2*k; j++){ // base case: first row - only integral volumes of drink 1 can be bought
		if(j%v[1] == 0){
			dp[1][j] = c[1] * (j/v[1]);
			drink_used[1][j] = 1;
			num_drinks[1][j] = 1;
		}
	}

	for(int i=1; i<=n; i++){ // base case: first col - 0 persons can be fed in 0 cost, using zero drinks
		dp[i][0] = 0;
		drink_used[i][0] = 0;
		num_drinks[i][0] = 0;
	}

	for(int i=2; i<=n; i++){
		for(int j=1; j<=2*k; j++){
			LLI left_cost; // coming from left
			int left_num;
			if((j-v[i] >= 0)){
				left_cost = dp[i][j-v[i]] + c[i];
				left_num = num_drinks[i][ j-v[i] ] + ( drink_used[i][ j-v[i] ] != i );
			} else {
				left_cost = INF;
				left_num = MINUS_INF;
			}

			LLI top_cost = dp[i-1][j]; // coming from top
			int top_num = num_drinks[i-1][j];

			if(left_cost < top_cost){
				dp[i][j] = left_cost;
				drink_used[i][j] = i;
				num_drinks[i][j] = left_num;
			}
			else if(left_cost == top_cost){
				dp[i][j] = left_cost;
				if(left_num > top_num){
					drink_used[i][j] = i;
					num_drinks[i][j] = left_num;
				}
				else{
					drink_used[i][j] = drink_used[i-1][j];
					num_drinks[i][j] = top_num;
				}
			}
			else{
				dp[i][j] = top_cost;
				drink_used[i][j] = drink_used[i-1][j];
				num_drinks[i][j] = top_num;
			}
		}
	}

	/*cout << "dp:\n";
	for(int i=1; i<=n; i++){
		for(int j=1; j<=2*k; j++){
			cout << dp[i][j] << " ";
		}
		cout << endl;
	}

	cout << "INF=" << INF << endl;*/

	LLI ans = dp[n][k];
	int num = num_drinks[n][k];
	for(int j=k+1; j<=2*k; j++){ // as drinks to be made is at least k litres !!
		if(dp[n][j] < ans){
			ans = dp[n][j];
			num = num_drinks[n][j];
		} 
		else if (dp[n][j] == ans){
			num = max(num, num_drinks[n][j]);
		}
	}

	//num = maxDrinks(c, n, ans);

	cout << std::fixed << setprecision(0) << ans << " " << num <<  endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	
	int t; 
	cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}