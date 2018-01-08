#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
#include <tuple>

using namespace std;

typedef long long int lli;

#define INF numeric_limits<lli>::max()
#define MINUS_INF numeric_limits<lli>::min()

vector<vector<int> > flr;
vector<vector<lli> > memo; // maximum num of 2*2 matrices we can place on top of the first i rows where i-th row is contrained
						   // as per the bitmask
unsigned int h, w, limit;

// count the no. of set bits in a positive integer
unsigned int countSetBits(unsigned int num) {
   unsigned int count = 0;
   while (num) {
      // bitwise AND operation to check if the
      // leftmost bit is set or not
      // if set, increment the count
      count += (num & 1);
      // left shift the nm by one position
      num >>= 1;
   }
   return count;
}

unsigned int negateBits(unsigned int num){
	for(int i=0; i<w; i++){
		num = num ^ (1<<i);
	}

	return num;
}

unsigned int compatible_tiles(unsigned int mask, int cur_row){
	unsigned int num_tiles = 0;
	for(int i=0; i<w-1; i++){
		unsigned int bits0 = (1<<i);
		unsigned int bits1 = (1<<(i+1) );

		if(flr[cur_row][i] == 1 && flr[cur_row][i+1] == 1 &&
			flr[cur_row-1][i] == 1 && flr[cur_row-1][i+1] == 1){ // floor plan permits 1 tile
			if( ((mask & bits0) != 0) && ((mask & bits1) != 0) ){
				num_tiles++;
				i++; // next col used in current tile
			}
		}
	}

	return num_tiles;
}

lli f(int row, unsigned int mask){
	if(memo[row][mask] != -1){
		return memo[row][mask];
	}

	// base cases
	if(row == 0){ // no tile fits in a single row
		return 0;
	}

	if(mask == 0){
		lli max_val = MINUS_INF;
		return f(row-1, limit-1); // we do not use the new row at all. Return the best result till previous row
	}

	unsigned int negated_bits = negateBits(mask);
	unsigned int num_tiles = compatible_tiles(mask, row);
	lli ret = f(row-1, negated_bits) + num_tiles; // tiles fit in row i and i-1

	for(int j=0; j<w; j++){
		if(mask & (1<<j) ){
			lli new_mask = mask & (~(1<<j)); // if j-th bit is set, unset it
			ret = max( ret, f(row, new_mask) );
		}
	}

	memo[row][mask] = ret;
	return ret;
}

void testcase(){
	cin >> h >> w;
	if(h>2 && w>2){
		flr = vector<vector<int> >(h-2, vector<int>(w-2));
	}

	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			int cell;
			cin >> cell;
			
			if(i>0 && i < h-1 && j>0 && j < w-1){
				flr[i-1][j-1] = cell;
			}
		}
	}

	if(w <= 2 || h <= 2){
		cout << 0 << endl;
		return;
	}

	w = w-2;
	h = h-2;
	limit = (1<<w);

	/*cout << "Floor:" << endl;
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			cout << flr[i][j];
		}
		cout << endl;
	}*/

	memo = vector<vector<lli> >(h, vector<lli>( limit, -1 ));
	//cout << "limit=" << limit << endl;
	cout << f(h-1, limit-1) << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}
}