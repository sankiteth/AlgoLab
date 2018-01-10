#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

#define MINUS_INF numeric_limits<long>::min()

int num_zones, num_jobs;
vector<int> ticket_cost;
vector<int> reward;
vector<int> jobs_num_zones;
vector<vector<int> > jobs;
//vector<long> memo;
unordered_map <pair<int, vector<bool> >, long, boost::hash< pair<int, vector<bool> > > > memo;

long f(int job, vector<bool>& zones_used, string str){
	if(memo.find(make_pair(job, zones_used)) != memo.end()){
		return memo[make_pair(job, zones_used)];
	}

	if(job == 0){ // 0 jobs to use means no profit
		return 0;
	}

	vector<bool> excluded_zones_used = zones_used;
	long excluded = f(job-1, excluded_zones_used, str+"  ");
	long included = 0;

	if(jobs[job].size() == 0){ // this job does not require any zone
		included = reward[job] + excluded;
		memo[make_pair(job, zones_used)] = included;

		return included;
	}
	
	// possibility of including the current job
	vector<bool> included_zones_used(zones_used);
	long cost = 0;
	for(int i=0; i < jobs_num_zones[job]; i++){
		if(!included_zones_used[ jobs[job][i] ]){
			included_zones_used[ jobs[job][i] ] = true;
			cost += ticket_cost[ jobs[job][i] ];
		}
	}

	included = f(job-1, included_zones_used, str+"  ") + reward[job] - cost;

	if(included >= excluded){ // include the current job then
		zones_used = included_zones_used;
	}
	else{
		zones_used = excluded_zones_used;
	}

	memo[make_pair(job, zones_used)] = max(included, excluded);
	return max(included, excluded);
}

void testcase(){
	cin >> num_zones >> num_jobs;

	ticket_cost = vector<int>(num_zones+1);
	for(int i=1; i<=num_zones; i++){
		cin >> ticket_cost[i];
	}

	reward = vector<int>(num_jobs+1);
	for(int i=1; i<=num_jobs; i++){
		cin >> reward[i];
	}

	jobs_num_zones = vector<int>(num_jobs+1);
	jobs = vector<vector<int> >(num_jobs+1, vector<int>());
	for(int i=1; i<=num_jobs; i++){
		cin >> jobs_num_zones[i];
		for(int j=0; j < jobs_num_zones[i]; j++){
			int z; cin >> z;
			jobs[i].push_back(z+1); // convert to 1-based index
		}
	}

	/*cout << "jobs:\n";
	for(int i=1; i<=num_jobs; i++){
		for(int j=0; j < jobs[i].size(); j++){
			cout << jobs[i][j] << " ";
		}
		cout << endl;
	}*/

	vector<bool> zones_used(num_zones+1, false);
	//memo = vector<long>(num_jobs+1, -1);
	memo.clear();
	cout << f(num_jobs, zones_used, "") << endl;
}

int main(){
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}