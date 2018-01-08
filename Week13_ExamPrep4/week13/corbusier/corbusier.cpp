#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <boost/functional/hash.hpp>

#define MINUS_INF numeric_limits<int>::min()
#define INF numeric_limits<int>::max()

using namespace std;

unordered_map <pair<int, int>, bool, boost::hash<pair<int, int> > > memo;
int num, rem, divisor;
vector<int> heights(num);

// subset sum with sum modulo
bool f(int i, int j){
	if(j < 0){
		j = (divisor+j)%divisor;
	}

	if(memo.find(make_pair(i,j)) != memo.end()){
		return memo[make_pair(i,j)];
	}

	if(i == 1){
		return (heights[i] % divisor) == j;  
	}

	bool ret = f(i-1, j) || f(i-1, (j - heights[i])	);
	memo[make_pair(i, j)] = ret;
	return ret;
}

void testcase(){ 
	cin >> num >> rem >> divisor;

	heights = vector<int>(num+1);
	for(int i=1; i<=num; i++){
		int h;
		cin >> h;
		heights[i] = h%divisor; // as sum of modulo = modulo of sum
	}

	 // memo[i,j] = if it is possible to make sum=(j mod divisor) with subset of elements from 1 to i
	memo = unordered_map<pair<int, int>, bool, boost::hash<pair<int, int> > >();
	bool ret = f(num, rem);

	if(ret){
		cout << "yes\n";
	}
	else{
		cout << "no\n";
	}
}

int main(){
	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}