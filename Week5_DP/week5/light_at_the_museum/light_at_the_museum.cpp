#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
#include <utility>

using namespace std;

void brute_force()
{
	int N,M; cin >> N >> M;
	vector<int> target(M);
	vector<vector<int> > switch_lights_on(N, vector<int>(M));
	vector<vector<int> > switch_lights_off(N, vector<int>(M));
	for (int i = 0; i < M; ++i)
	{
		cin >> target[i];
	}

	vector<int> room_lights_on(M, 0);
	vector<int> room_lights_off(M, 0);

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			int on, off; cin >> on >> off;
			switch_lights_on[i][j] = on;
			switch_lights_off[i][j] = off;

			room_lights_on[j] += on;
			room_lights_off[j] += off;
		}
	}	

	int min_flicks = N+1;

	// check all subsets of switches
	for (int s = 0; s < (1<<N); ++s)
	{
		vector<int> temp(room_lights_on);
		int num_flicks = 0;
		for (int i = 0; i < N; ++i)
		{
			// ith switch is flicked
			if (s & (1 << i))
			{
				num_flicks++;
				for (int j = 0; j < M; ++j)
				{
					temp[j] += (switch_lights_off[i][j] - switch_lights_on[i][j]); 
				}
			}
		}

		if (temp == target)
		{
			if (num_flicks < min_flicks)
			{
				min_flicks = num_flicks;
			}
		}
	}

	if (min_flicks == N+1)
	{
		cout << "impossible\n";
	}
	else
	{
		cout << min_flicks << endl;
	}
}

void split_list()
{
	int N,M; cin >> N >> M;
	vector<int> target(M);
	vector<vector<int> > switch_lights_on(N, vector<int>(M));
	vector<vector<int> > switch_lights_off(N, vector<int>(M));
	for (int i = 0; i < M; ++i)
	{
		cin >> target[i];
	}

	vector<int> room_lights_on(M, 0);
	vector<int> room_lights_off(M, 0);

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			int on, off; cin >> on >> off;
			switch_lights_on[i][j] = on;
			switch_lights_off[i][j] = off;

			room_lights_on[j] += on;
			room_lights_off[j] += off;
		}
	}	

	// Divide into 2 sets of equal sizes
	int N_1 = N/2;
	int N_2 = N - N_1;

	// check all subsets of first set
	map<vector<int>, int> l1_light_states;
	for (int s = 0; s < (1<<N_1); ++s)
	{
		vector<int> state_change(M, 0);
		int num_flicks = 0;
		for (int i = 0; i < N_1; ++i)
		{
			// ith switch is flicked
			if (s & (1 << i))
			{
				num_flicks++;
				for (int j = 0; j < M; ++j)
				{
					state_change[j] += (switch_lights_off[i][j] - switch_lights_on[i][j]); 
				}
			}
		}

		if(l1_light_states.find(state_change) == l1_light_states.end())
		{
			l1_light_states[state_change] = num_flicks;
		}
		else
		{
			// may be different subsets of switch flicks produce the same state changes in all the rooms
			if (l1_light_states[state_change] > num_flicks)
			{
				l1_light_states[state_change] = num_flicks;
			}
		}
	}

	int min_flicks = N+1;

	// check all subsets of second set
	map<vector<int>, int> l2_light_states;
	for (int s = 0; s < (1<<N_2); ++s)
	{
		vector<int> current_on(room_lights_on);
		int num_flicks = 0;

		// ith switch in the second set
		for (int i = 0; i < N_2; ++i)
		{
			// ith switch is flicked
			if (s & (1 << i))
			{
				num_flicks++;
				for (int j = 0; j < M; ++j)
				{
					current_on[j] += (switch_lights_off[N_1+i][j] - switch_lights_on[N_1+i][j]); 
				}
			}
		}

		for (int i = 0; i < M; ++i)
		{
			current_on[i] = target[i] - current_on[i];
		}

		if (l1_light_states.find(current_on) != l1_light_states.end())
		{
			if (l1_light_states[current_on]+num_flicks < min_flicks)
			{
				min_flicks = l1_light_states[current_on]+num_flicks;
			}
		}
	}

	if (min_flicks == N+1)
	{
		cout << "impossible\n";
	}
	else
	{
		cout << min_flicks << endl;
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		split_list();
	}

	return 0;
}