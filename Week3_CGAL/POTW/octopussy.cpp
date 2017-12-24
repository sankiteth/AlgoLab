#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>

using namespace std;

void mark_all_child_visited(int cur_node, bool* visited, int n)
{
	queue<int> q;

	q.push( cur_node );
	while(!q.empty())
	{
		int front = q.front();
		visited[ front ] = true;

		q.pop();

		if ( (2*front + 2) < n )
		{
			q.push(2*front + 1);
			q.push(2*front + 2);
		}
	}
}

void reduce_diffused_children_till_root(int cur_node, int num_child_diffused, vector<pair<int, int> > & t_numChild)
{
	int node = cur_node;

	while(node > 0)
	{
		node = (node - 1)/2;
		t_numChild[ node ].second -= num_child_diffused;
	}	
}

void octopussy()
{
	int n; cin >> n;

	vector<pair<int, int> > t_numChild;
	vector<pair<int, int> > t_ind;

	for (int i = 0; i < n; ++i)
	{
		int t_i;
		cin >> t_i;
		t_numChild.push_back(make_pair(t_i, 0));
		t_ind.push_back(make_pair(i, t_i));
	}

	// non leaves
	for (int i = n/2 - 1; i >= 0; --i)
	{
		t_numChild[i].second = 2 + (t_numChild[2*i+1].second * 2);
	}

	sort(t_ind.begin(), t_ind.end(), 
			[] (pair<int, int> x, pair<int, int> y) -> bool
			{
				return x.second <= y.second;
			}
		);

	int time = 1;
	bool visited[n] = {false};

	for (int i = 0; i < n; ++i)
	{
		int cur_node = t_ind[i].first;
		if( !visited[cur_node] )
		{
			visited[cur_node] = true;

			// if all children diffused.
			if (t_numChild[cur_node].second == 0)
			{
				if (time <= t_numChild[cur_node].first)
				{
					time++;

					// num_child of parent and grand parents reduced by 1.
					reduce_diffused_children_till_root(cur_node, 1, t_numChild);
				}
				else
				{
					cout << "no" << endl;
					return;
				}
			}
			else
			{
				if ( (time + t_numChild[ cur_node ].second)  <= t_numChild[ cur_node ].first )
				{
					time += 1 + t_numChild[ cur_node ].second;

					// num_child of parent and grand parents reduced by 1 + t_numChild[ cur_node ].second
					int num_child_diffused = 1 + t_numChild[ cur_node ].second;
					t_numChild[ cur_node ].second = 0;

					reduce_diffused_children_till_root(cur_node, num_child_diffused, t_numChild);

					// Mark all children as visited
					mark_all_child_visited(cur_node, visited, n);
				}
				else
				{
					cout << "no" << endl;
					return;
				}
			}
		}
	}

	cout << "yes" << endl;
}

int main()
{
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		octopussy();
	}
}