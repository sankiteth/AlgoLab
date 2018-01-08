#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

#define INF numeric_limits<int>::max()
#define MINUS_INF numeric_limits<int>::min()

void testcase(){
	int n; cin >> n;
	vector<int> parasols(n);
	for(int i=0; i<n; i++){
		cin >> parasols[i];
	}

	sort(parasols.begin(), parasols.end());

	/*cout << "parasols:\n";
	for(int i=0; i<n; i++){
		cout << parasols[i] << " ";
	}
	cout << endl;*/

	vector<tuple<int, int, int> > bars;
	int l, h, num;
	for(auto it=parasols.begin(); it != parasols.end(); it++){

		// search for an element <= (*it + 200) in parasol
		vector<int>::iterator high = upper_bound(parasols.begin(), parasols.end(), *it + 200); // returns first element greater
																								 // than the argument
		l = *it;
		if(high == parasols.end()){
			h = parasols[n-1];
			num = high - it;
			bars.push_back(make_tuple(l,h,num));
			break;
		}
		else{
			high--;
			h = *high;
			num = high - it + 1;
		}

		bars.push_back(make_tuple(l,h,num));
	}

	/*cout << "bars:\n";
	for(auto it = bars.begin(); it!=bars.end(); it++){
		cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << endl;
	}*/

	int max_num = MINUS_INF;
	for(auto it = bars.begin(); it!=bars.end(); it++){
		max_num = max(max_num, get<2>(*it) );
	}

	vector<tuple<int, int, int> > max_bars;
	int min_max_dist = INF;
	//cout << "min_max_dist:\n";
	for(auto it = bars.begin(); it!=bars.end(); it++){
		l = get<0>(*it);
		h = get<1>(*it);
		num = get<2>(*it);

		if(num == max_num){
			max_bars.push_back(*it);
			int centre = l + (h - l)/2;
			//cout << centre << endl;
			min_max_dist = min(min_max_dist, max(centre-l, h-centre));
		}
	}

	/*cout << "max_bars:\n";
	for(auto it = max_bars.begin(); it!=max_bars.end(); it++){
		cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << endl;
	}*/

	int first = true;
	cout << max_num << " " << min_max_dist << endl;
	int prev_centre_1 = MINUS_INF, prev_centre_2 = MINUS_INF;
	for(auto it = max_bars.begin(); it!=max_bars.end(); it++){
		l = get<0>(*it);
		h = get<1>(*it);
		num = get<2>(*it);

		int centre_1 = l + (h - l)/2;
		int dist = max(centre_1-l, h-centre_1);

		int centre_2 = MINUS_INF;
		if( abs((centre_1-l) - (h-centre_1)) == 1 ){
			centre_2 = centre_1 + 1;
		}

		if(dist == min_max_dist && centre_1 != prev_centre_1){
			if(first){
				cout << centre_1;
				if(centre_2 != MINUS_INF){
					cout << " " << centre_2;
				}
				first = false;
			}
			else{
				cout << " " << centre_1;
				if(centre_2 != MINUS_INF){
					cout << " " << centre_2;
				}
			}
		}

		prev_centre_1 = centre_1;
	}

	cout << endl;
}

int main(){
	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}