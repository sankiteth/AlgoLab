// High School Teams
// Use Split and List to try all combinations.
// For each half of the players, use recursive back tracking
// Each list is a vector of vector<int, int> = { strength difference, num players on bench }
// Use binary search to count the number of valid teams

#include <iostream>
#include <vector>
#include <algorithm>

#define REP(i, a, b) for(int i = a; i < b; ++i)

using std::cin; using std::cout;
typedef std::vector<int> vi;
typedef std::vector<vi> vvi;

vi skills;
int n, k;

void back_track(int id, int ubound, vvi &F, vvi &teams){
  if(teams[2].size() > k) return;
  if (id >= ubound){
    std::vector<int> tuple(2, 0);
    REP(i, 0, teams[0].size()){ tuple[0] += skills[teams[0][i]]; }
    REP(i, 0, teams[1].size()){ tuple[0] -= skills[teams[1][i]]; }
    tuple[1] = teams[2].size();
    F.push_back(tuple);
    return;
  }

  for(int i = 0; i < 3; ++i){
    teams[i].push_back(id); // check possibilities if player id is added to team i
    back_track(id + 1, ubound, F, teams);
    teams[i].pop_back(); // take player id off the team 
  }
}

long long testcase(){
  skills.resize(n);
  for(int i = 0; i < n; ++i){ cin >> skills[i]; }

  vvi F1, assignment(3), F2, assignment2(3);
  back_track(0, n/2, F1, assignment);
  back_track(n/2, n, F2, assignment2);

  std::sort(F2.begin(), F2.end());
  
  long long n_teams = 0;
  for(int index = 0; index < F1.size(); ++index){
    vi member = F1[index];
    vi lower = { -member[0], 0}, // search for teams that use between 0 and k-member[1] players
      upper = { -member[0], k - member[1] };
    vvi::iterator it1, it2;
    it1 = std::lower_bound(F2.begin(), F2.end(), lower);
    it2 = std::upper_bound(F2.begin(), F2.end(), upper);
    long long counter = it2 - it1;
    n_teams += counter;
  }
  return n_teams;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    cin >> n >> k;
    cout << testcase() << "\n";
  }
  return 0;
}