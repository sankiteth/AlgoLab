#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>

using namespace std;

#define MINUS_INF numeric_limits<long>::min()

int num_zones, num_jobs;
vector<int> ticket_cost;
vector<int> reward;
vector<int> jobs_num_zones;
vector<vector<int> > jobs;
vector<vector<long> > memo;

long f(int zone, int job, vector<bool>& zones_used){
	//cout << "zone=" << zone << " job=" << job << endl;
	/*if(memo[zone][job] != -1){
		return memo[zone][job];
	}*/

	if(zone == 0 || job == 0){ // 0 jobs or 0 zones to use means no profit
		return 0;
	}

	vector<bool> excluded_zones_used(zones_used);
	long excluded = f(zone, job-1, excluded_zones_used);
	long included = 0;

	if(jobs[job].size() == 0){ // this job does not require any zone
		included = reward[job] + excluded;
		memo[zone][job] = included;
		return included;
	}

	if( jobs[job][ jobs_num_zones[job]-1 ] > zone ){ // do nothing - this job cannot be included now
		memo[zone][job] = excluded;
		return excluded;
	}

	vector<bool> included_zones_used = zones_used;
	if(jobs[job][ jobs_num_zones[job]-1 ] <= zone){
		long cost = 0;
		if(!included_zones_used[zone]){ // zone being used first time
			cost += ticket_cost[zone];
			included_zones_used[zone] = true;
		}

		long sub_problem = f(zone, job-1, included_zones_used);

		bool all_zones_used = true;
		for(int i=0; i<jobs_num_zones[job]; i++){ // see if all zones needed for this job used
			if(!included_zones_used[ jobs[job][i] ]){
				all_zones_used = false;
				break;
			}
		}

		if(all_zones_used)
			included = sub_problem - cost + reward[job];
	}
	/*else{
		included = f( jobs[job][ jobs_num_zones[job]-1 ], job, included_zones_used);
	}*/

	if(included >= excluded){
		zones_used = included_zones_used;
		memo[zone][job] = included;
	}
	else{
		memo[zone][job] = excluded;
		zones_used = excluded_zones_used;
	}

	memo[zone][job] = max(included, excluded);
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
	memo = vector<vector<long> >(num_zones+1, vector<long>(num_jobs+1, -1));
	cout << f(num_zones, num_jobs, zones_used) << endl;
}

int main(){
	ios_base::sync_with_stdio(true);

	int t; cin >> t;
	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}