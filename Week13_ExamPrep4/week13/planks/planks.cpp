// Planks
// Split and list
// For each of the split list, we maintain a 4-tuple holding the sum
// In the search, we search for totalsum/4 - F1[i] in F2.
// Divide by 4! = 24 due to the counting of permutations

#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std; 

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;

int n;
vi planks;

// all divisions into 4 subsets
void backtrack(int id, int ubound, vvi &F, vvi &sides){
    if(id >= ubound){
        // tuple[i] is the total length of side i 
        vi tuple(4, 0);
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < sides[i].size(); ++j){ 
                tuple[i] += planks[sides[i][j]]; 
            }
        }

        F.push_back(tuple);
        return;
    }

    // Try adding id to all teams
    for(int i = 0; i < 4; ++i){
        sides[i].push_back(id);
        backtrack(id + 1, ubound, F, sides);
        sides[i].pop_back();
    }
}

long long testcase(){
    planks.resize(n);
    int sum = 0;
    for(int i = 0; i < n; ++i) { 
        cin >> planks[i]; 
        sum += planks[i]; 
    }

    if(sum % 4 != 0) {
        return 0; 
    }
    
    sum /= 4;
  
    vvi F1, F2, sides1(4), sides2(4);
    backtrack(0, n/2, F1, sides1);
    backtrack(n/2, n, F2, sides2);

    std::sort(F2.begin(), F2.end());
    long long counter = 0;

    for(int index = 0; index < F1.size(); ++index){
        vi member = F1[index];
        for(int i=0; i<4; i++){ 
            member[i] = sum - member[i]; 
        }
        vvi::iterator it1 = std::lower_bound(F2.begin(), F2.end(), member); // first occurence
        vvi::iterator it2 = std::upper_bound(F2.begin(), F2.end(), member); // one more than last occurence
        counter += (long long)(it2 - it1);
    }

    return counter / 24; // need combination not permutation, hence divide by 4 factorial
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    cin >> n;
    cout << testcase() << "\n";
  }
}