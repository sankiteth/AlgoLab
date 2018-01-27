#include <iostream>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <assert.h>

typedef long long ll;
int n, k;
ll skill[22], subset_skill[22];
std::vector<ll> first_set, second_set;
std::unordered_map<ll, ll> sums;

void read_test_case(){

	std::cin >> n >> k;
	for(int i=0;i<n;i++)
		std::cin >> skill[i];
}

void solve(){

	ll counter = 0;
	
	ll limit = 1 << n;
	for(int i=0;i<limit;i++){
		std::bitset<22> bits(i);
		if(bits.count() < n-k)
			continue;

		ll s = 0;
		int c = 0;
		for(int j=0;j<n;j++){
			if(i & 1 << j){
				s+=skill[j];
				subset_skill[c++] = skill[j];
			}
		}

		assert(c == bits.count());

		if(s%2 != 0)
			continue;

		ll target_sum = s/2;
		first_set.clear(); second_set.clear();
		for(int ii=0;ii<c;ii++){
			if(ii < c/2)
				first_set.push_back(subset_skill[ii]);
			else
				second_set.push_back(subset_skill[ii]);
		}

		int n_first = first_set.size(), n_second = second_set.size();
		sums.clear();

		ll limit_first = 1 << n_first;
		for(int ii=0;ii<limit_first;ii++){
			ll ss = 0;
			for(int jj=0;jj<n_first;jj++){
				if(ii & 1 << jj)
					ss += first_set[jj];				
			}

			if(sums.find(ss) == sums.end()){
				sums[ss] = 1;
			}
			else{
				sums[ss] += 1;
			}
		}

		ll limit_second = 1 << n_second;
		for(int ii=0;ii<limit_second;ii++){
			ll ss = 0;
			for(int jj=0;jj<n_second;jj++){
				if(ii & 1 << jj){
					ss += second_set[jj];
				}
			}

			if( sums.find(target_sum - ss) != sums.end() ){
				counter += sums[target_sum - ss];
			}
		}
	}

	std::cout << counter << std::endl;
}

void test_case(){
	read_test_case();
	solve();
}

void testcase_2(){
	read_test_case();

	ll counter = 0;

	for(int i=0; i<n; i++){ // divide students into 2 equal halves
		if(i<n/2){
			first_set.push_back(skill[i]);
		}
		else{
			second_set.push_back(skill[i]);
		}
	}

	int n_first = first_set.size();
	ll limit_first = 1 << n_first;
	std::vector<int, std::unordered_map<ll,ll> > memo(k+1);

	for(int i=0; i<limit_first; i++){ // all subsets of first half
		std::bitset<22> bits(i);
		int set_bits = bits.count();
		int off_bits = n_first - set_bits;
		if(off_bits > k){
			continue;
		}

		ll skill_sum = 0;
		for(int j=0;j<n_first;j++){
			if( i & (1 << j) ){
				skill_sum += skill[j];
			}
		}

		if(memo[off_bits].find(skill_sum) == memo[off_bits].end()){
			memo[off_bits][skill_sum] = 1;
		}
		else{
			memo[off_bits][skill_sum] += 1;
		}
	}

	int n_second = second_set.size();
	ll limit_second = 1 << n_second;

	for(int i=0; i<limit_second; i++){ // all subsets of second half
		std::bitset<22> bits(i);
		int set_bits = bits.count();
		int off_bits = n_second - set_bits;
		if(off_bits > k){
			continue;
		}
	}

}

int main(){
	std::ios_base::sync_with_stdio(false);

	int T; std::cin >> T; 
	while(T--) 
		test_case();

	return 0;
}