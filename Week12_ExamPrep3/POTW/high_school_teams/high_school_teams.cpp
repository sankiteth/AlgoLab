#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <bitset>

using namespace std;

vector<int> subset_sums;

void cal_subset_sums(vector<int>stren, int len){
	/*int N = 2;
	int A[] = {1, 2, 3, 4};
	int F[1<<N] = {0};
	for(int mask = 0;mask < (1<<N); ++mask){
		for(int i = 0;i < (1<<N); ++i){
			if((mask&i) == i){
				F[mask] += A[i];
			}
		}
	}

	for(int mask = 0;mask < (1<<N); ++mask){
		cout << "mask=" << bitset<3>(mask) << "  F=" << F[mask] << endl;
	}*/

	for (int s=0; s < 1<<len; ++s){ // iterate through all subsets
		int sum = 0;
		for (int i=0; i<len; ++i){
			if (s & (1<<i)) 
				sum += stren[i]; // if i - th element in subset
		}
		//cout << "set=" << bitset<4>(s) << " sum=" << sum << endl;
		subset_sums[s] = sum;
	}
}

int count_bits(int n){
	int ret = 0;
	while(n>0){
		n = n/2;
		ret++;
	}

	return ret;
}

int num_Ways(vector<int>& stren, int cur_set, int len, int num_off){
	//cout << "cur_set=" << cur_set << endl;
	if(cur_set == 0){
		return 1; // 1 way to partition empty set to 2 teams
	}

	int total_stren = subset_sums[cur_set];
	if(total_stren%2 != 0){ // total strength cannot be divided into 2 teams
		return 0;
	}

	int num_ways = 0;
	// for each subset of current set, calculate sum and check if strength is equally divided among 2 teams
	for(int i=0; i<=(cur_set); ++i){
		if((cur_set&i) == i && subset_sums[i] == total_stren/2){ // if i a subset of cur_set, and the sum in the subset is half
			num_ways++;											 // of available total strength.
			//cout << "i=" << bitset<10>(i) << " subset_sum=" << subset_sums[i] << "num_off=" << num_off << endl;
		}
	}


	return num_ways;
}

void create_combinations(int arr[], int n, int r, int index, int data[], int i)
{
    // Current cobination is ready, print it
    if (index == r)
    {
        for (int j=0; j<r; j++)
            printf("%d ",data[j]);
        printf("\n");
        return;
    }
 
    // When no more elements are there to put in data[]
    if (i >= n)
        return;
 
    // current is included, put next at next location
    data[index] = arr[i];
    combinationUtil(arr, n, r, index+1, data, i+1);
 
    // current is excluded, replace it with next (Note that
    // i+1 is passed, but index is not changed)
    combinationUtil(arr, n, r, index, data, i+1);
}

void testcase(){
	int n, k;
	cin >> n >> k;

	vector<int> stren(n);
	for(int i=0; i<n; i++){
		cin >> stren[i];
	}

	int index_arr[n];
	for(int i=0; i<n; i++){
		index_arr[i] = i;
	}

	vector<vector<int> > comb_masks(n+1, vector<int>()); // when k bits on, what are the combs
	for (int i=0; i < (1<<n); i++) { // i goes through all subsets of set with n elements
		int c=0;
        for (int j=0; j<n; j++){ // masking the j'th bit as j goes through all the bits, count the number of 1 bits.
        	if (i & (1<<j)){
        		c++;
        	}
        }

    	//cout << "\ni=" << bitset<6>(i) << "\nc=" << c << endl;
		// if number of set bits is equal to k, store the combination
    	comb_masks[c].push_back(i); // c bits on
    }
    
    /*cout << "comb_masks:\n";
    for(int i=0; i<=n; i++){
    	for(int j=0; j < comb_masks[i].size(); j++){
    		cout << bitset<10>( comb_masks[i][j] ) << endl;
    	}

    	cout << endl;
    }
*/
    subset_sums = vector<int>(1<<n);
    cal_subset_sums(stren, n);

    /*cout << "subset_sums:\n";
    for(int i=0; i< 1<<n; i++){
		cout << "subset=" << bitset<5>(i) << "  sum=" << subset_sums[i] << endl;
    }*/

    int num_ways = 0;
    for(int i=0; i<=k; i++){ // number of bits off
    	for(int j=0; j<comb_masks[n-i].size(); j++){ // n-i = number of bits on
    		//cout << "i=" << i << " j=" << j << endl;
    		num_ways += num_Ways(stren, comb_masks[n-i][j], n, i);	
    	}
    }

    cout << num_ways << endl;
}

int main(){
	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}