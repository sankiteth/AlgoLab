#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>
#include <set>
#include <boost/functional/hash.hpp>

using namespace std;

#define INF numeric_limits<double>::max()
#define MINUS_INF numeric_limits<int>::min()

void vector_insert(vector<int>& v, int elem){
	if (find(v.begin(), v.end(), elem) == v.end()) {
  		v.push_back(elem);
	}
}

bool vector_contains(vector<int>& v, int elem){
	if (find(v.begin(), v.end(), elem) == v.end()) {
  		return false;
	}
	else{
		return true;
	}
}

void testcase(){
	int n, k;
	cin >> n >> k;

	int c[n+1], v[n+1];
	for(int i=1; i<=n; i++){
		cin >> c[i] >> v[i];
	}

	vector<vector<double> > dp(n+1, vector<double>(2*k+1, INF) );
	vector<vector<set<int> > > drinks(n+1, vector<set<int> >(2*k+1, set<int>()));


	for(int j=1; j<=2*k; j++){ // base case: first row - only integral volumes of drink 1 can be bought
		if(j%v[1] == 0){
			dp[1][j] = c[1] * (j/v[1]);
			drinks[1][j].insert(1);
		}
	}

	for(int i=1; i<=n; i++){ // base case: first col - 0 persons can be fed in 0 cost, using zero drinks
		dp[i][0] = 0;
	}

	for(int i=2; i<=n; i++){
		for(int j=1; j<=2*k; j++){
			if((j-v[i] >= 0) && ( dp[i][j-v[i]] + c[i] < dp[i-1][j] ) ) {
				dp[i][j] = dp[i][j-v[i]] + c[i]; // use v[i] litres of ith drink using c[i] money
				drinks[i][j].insert(drinks[i][j-v[i]].begin(), drinks[i][j-v[i]].end());
				drinks[i][j].insert(i);
			} 
			else if((j-v[i] >= 0) && ( dp[i][j-v[i]] + c[i] == dp[i-1][j] ) ) { // use the option with higher number of drinks
				dp[i][j] = dp[i][j-v[i]] + c[i];

				if(drinks[i-1][j].size() < drinks[i][j-v[i]].size()){
					drinks[i][j].insert(drinks[i][j-v[i]].begin(), drinks[i][j-v[i]].end());
					drinks[i][j].insert(i);
				}
				else if(drinks[i-1][j].size() == drinks[i][j-v[i]].size()){

					if(drinks[i][j-v[i]].find(i) == drinks[i][j-v[i]].end() ){
						drinks[i][j].insert(drinks[i][j-v[i]].begin(), drinks[i][j-v[i]].end());
						drinks[i][j].insert(i);
					} else {
						drinks[i][j].insert(drinks[i-1][j].begin(), drinks[i-1][j].end());	
					}
				}
				else {
					drinks[i][j].insert(drinks[i-1][j].begin(), drinks[i-1][j].end());	
				}
			}
			else { // do not use drink i at all
				dp[i][j] = dp[i-1][j];
				drinks[i][j].insert(drinks[i-1][j].begin(), drinks[i-1][j].end());
			}
		}
	}

	/*cout << "dp:\n";
	for(int i=1; i<=n; i++){
		for(int j=1; j<=2*k; j++){
			cout << dp[i][j] << " ";
		}
		cout << endl;
	}*/

	double ans = INF;
	int num = MINUS_INF;
	for(int j=k; j<=2*k; j++){ // as drinks to be made is at least k litres !!
		if(dp[n][j] < ans){
			ans = dp[n][j];
			num = drinks[n][j].size();
		} 
		else if (dp[n][j] == ans){
			num = max(num, (int)drinks[n][j].size() );
		}
	}

	cout << ans << " " << num <<  endl;
}

int main(){
	int t; 
	cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}