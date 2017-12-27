#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

#define MINUS_INF numeric_limits<int>::min()

vector<vector<long> > memo;
vector<vector<int> > a;
vector<vector<pair<int, int> > > sol;

unordered_map < tuple<int, int, vector<vector<bool> > >, 
				long, 
				boost::hash<tuple<int, int, vector<vector<bool> > > > 
			> mem;


long f(int i, int j){
	if(memo[i][j] != -1){
		return memo[i][j];
	}

	int max_val = MINUS_INF;

	int i_s[] = {i-1, i  };
	int j_s[] = {j  , j-1};

	for(int k=0; k<2; k++){
		int val = f(i_s[k], j_s[k]);
		if(val > max_val){
			max_val = val;
			sol[i][j] = make_pair(i_s[k], j_s[k]);
		}
	}

	long ret = max_val + a[i][j];
	memo[i][j] = ret;

	return ret;
}

long g(tuple<int, int, vector<vector<bool> > > state){
	if(mem.find(state) != mem.end()){
		return mem[state];
	}

	for()

	
	return 0;
}

void testcase(){
	int n; cin >> n;

	memo = vector<vector<long> >(n, vector<long>(n, -1));
	a    = vector<vector<int> >(n, vector<int>(n, 0));
	sol  = vector<vector<pair<int, int> > >(n, vector<pair<int, int> >(n, make_pair(-1, -1) ));

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cin >> a[i][j];
		}
	}

	// base cases
	memo[0][0] = a[0][0];
	for(int i=1; i<n; i++){
		memo[0][i] = memo[0][i-1] + a[0][i];
		sol[0][i] = make_pair(0, i-1);
	}

	for(int i=1; i<n; i++){
		memo[i][0] = memo[i-1][0] + a[i][0];
		sol[i][0] = make_pair(i-1, 0);
	}

	int ans = f(n-1, n-1);

	cout << "sol\n";
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cout << sol[i][j].first << "," << sol[i][j].second << " ";
		}
		cout << endl;
	}

	// return input array for return of mario
	pair<int, int> p = make_pair(n-1, n-1);
	do{
		cout << sol[p.first][p.second].first << "," << sol[p.first][p.second].second << endl;
		a[p.first][p.second] = 0;
		
		p = sol[p.first][p.second];

		cout << "x=" << p.first << " y=" << p.second << endl;
	}while( p.first>=0 && p.second>=0 );

	cout << "a:\n";
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cout << a[i][j] << " ";
		}
		cout << endl;
	}

	memo = vector<vector<long> >(n, vector<long>(n, -1));
	sol  = vector<vector<pair<int, int> > >(n, vector<pair<int, int> >(n, make_pair(-1, -1) ));

	// base cases
	memo[0][0] = a[0][0];
	for(int i=1; i<n; i++){
		memo[0][i] = memo[0][i-1] + a[0][i];
		sol[0][i] = make_pair(0, i-1);
	}

	for(int i=1; i<n; i++){
		memo[i][0] = memo[i-1][0] + a[i][0];
		sol[i][0] = make_pair(i-1, 0);
	}

	ans += f(n-1, n-1);

	cout << ans << endl;
}

void new_approach(){

}

int main(){
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}