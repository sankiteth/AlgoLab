#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <utility>
#include <limits>
#include <queue>

#define p_LL_i pair<long long int, int> 

using namespace std;

bool is_valid(vector<pair<int, long long int> > merged, int l, int r, int n)
{
	bool contains[n];
	for (int i = 0; i < n; ++i)
	{
		contains[i] = false;
	}
	for (int i = l; i <= r; ++i)
	{
		contains[merged[i].first] = true;
	}

	bool is_val = true;
	for (int i = 0; i < n; ++i)
	{
		is_val = is_val && contains[i];
	}

	return is_val;
}

void search_snippets_1()
{
	int n; cin >> n;
	int m[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> m[i];
	}

	//vector<vector<pair<int, long long int> > > p( n, vector<pair<int, long long int> >() );

	vector<pair<int, long long int> > merged;

	for (int i = 0; i < n; ++i)
	{
		vector<pair<int, long long int> > v(m[i]);

		for (int j = 0; j < m[i]; ++j)
		{
			long long int p_ij; cin >> p_ij;

			v.push_back(make_pair(i, p_ij) );
		}

		// merge the vectors to form a single vector
		vector<pair<int, long long int> > temp(merged.size() + v.size());

		merge(merged.begin(), merged.end(), v.begin(), v.end(),  temp.begin(), 
				[ ]( pair<int, long long int>& lhs, pair<int, long long int>& rhs )
				{
				   return lhs.second < rhs.second;
				}
			);

		merged = temp;
	}

	

	// perform sliding window on merged
	int l = 0, r = 0;
	long long int cur_len, best_len = merged[merged.size()-1].second - merged[0].second + 1;
	set<int> valid;

	while(l < merged.size())
	{
		cur_len = merged[r].second - merged[l].second + 1;

		bool is_val = is_valid(merged, l, r, n);

		if (is_val && (cur_len < best_len) )
		{
			best_len = cur_len;
		}

		if (!is_val && r < (merged.size()-1) )
		{
			r++;
		}
		else
		{
			l++;
		}
	}

	cout << best_len << endl;
}

// works only for n=2
void search_snippets_2()
{
	int n; cin >> n;
	int m[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> m[i];
	}

	vector<vector<long long int> > p( n, vector<long long int>() );

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m[i]; ++j)
		{
			long long int p_ij; cin >> p_ij;

			p[i].push_back(p_ij);
		}
	}

	// find min, 2nd min and max

	long int min_1;
	long int min_2;
	long int max_1;

	int min_1_ind_i;
	int min_2_ind_i;
	int max_1_ind_i;

	if (p[0][0] < p[1][0])
	{
		min_1 = p[0][0];
		min_2 = p[1][0];

		min_1_ind_i = 0;
		min_2_ind_i = 1;
	}
	else
	{
		min_1 = p[1][0];
		min_2 = p[0][0];

		min_1_ind_i = 1;
		min_2_ind_i = 0;
	}

	max_1 = min_2;
	max_1_ind_i = min_2_ind_i;

	for (int i = 2; i < n; ++i)
	{
		if (p[i][0] < min_1) // p[i][0] < min_1 < min_2
		{
			min_2 = min_1;
			min_1 = p[i][0];

			min_2_ind_i = min_1_ind_i;
			min_1_ind_i = i;
		}
		else if (p[i][0] < min_2) // min_1 < p[i][0] < min_2
		{
			min_2 = p[i][0];

			min_2_ind_i = i;
		}

		if (p[i][0] > max_1)
		{
			max_1 = p[i][0];
			max_1_ind_i = i;
		}
	}

	// sliding window on all the vectors
	int min_1_ind_j = 0;
	int min_2_ind_j = 0;
	int max_1_ind_j = 0;

	long long int ans = max_1 - min_1 + 1;

	while(min_1_ind_j < m[min_1_ind_i])
	{
		if (max_1-min_1+1 < ans)
		{
			ans = max_1 - min_1 + 1;
		}

		if (min_1_ind_j == m[min_1_ind_i]-1)
		{
			break;
		}

		int new_val = p[min_1_ind_i][min_1_ind_j+1];

		// find min, second min and max of new_val, min_2, max_1

		if(new_val < min_2) // new_val < min_2 < max_1
		{
			min_1 = new_val;

			min_1_ind_j++;
		}
		else if (new_val < max_1) // min_2 < new_val < max_1
		{
			min_1 = min_2;
			min_2 = new_val;

			swap(min_1_ind_i, min_2_ind_i);

			swap(++min_1_ind_j, min_2_ind_j);
		}
		else // min_2 < max_1 < new_val
		{
			min_1 = min_2;
			min_2 = max_1;
			max_1 = new_val;

			int temp = min_1_ind_i;
			min_1_ind_i = min_2_ind_i;
			min_2_ind_i = max_1_ind_i;
			max_1_ind_i = temp;

			temp = min_1_ind_j+1;
			min_1_ind_j = min_2_ind_j;
			min_2_ind_j = max_1_ind_j;
			max_1_ind_j = temp;
		}

		// only 2 words present -> assign max to second min.
		if (n == 2)
		{
			min_2 = max_1;

			min_2_ind_i = max_1_ind_i;
			min_2_ind_j = max_1_ind_j;
		}

	}

	cout << ans << endl;
}

// works!!!
// Let M = average number of words for each of the N separate word in query.
// O(MMN)
void search_snippets_3()
{
	int n; cin >> n;
	int m[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> m[i];
	}

	vector<vector<long long int> > p( n, vector<long long int>() );

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m[i]; ++j)
		{
			long long int p_ij; cin >> p_ij;

			p[i].push_back(p_ij);
		}
	}

	long long int min = numeric_limits<long long int>::max();
	long long int max = numeric_limits<long long int>::lowest();

	int min_ind_i;

	for (int i = 0; i < n; ++i)
	{
		if (p[i][0] < min)
		{
			min = p[i][0];

			min_ind_i = i;
		}

		if (p[i][0] > max)
		{
			max = p[i][0];
		}
	}

	// Do sliding window now
	vector<int> ind_j(n, 0);

	long long int ans =  max - min + 1;

	while( ind_j[min_ind_i] < m[min_ind_i] )
	{//cout << endl << min << " " << max << " " << min_ind_i << endl;

		if ( (max - min + 1) < ans )
		{
			ans = max - min + 1;
		}

		if (ind_j[min_ind_i] == m[min_ind_i]-1 )
		{
			break;
		}

		ind_j[min_ind_i]++;

		min = numeric_limits<long long int>::max();
		max = numeric_limits<long long int>::lowest();

		for (int i = 0; i < n ; ++i)
		{

			if (p[i][ ind_j[i] ] < min)
			{
				min = p[i][ ind_j[i] ];
				min_ind_i = i;
			}

			if (p[i][ ind_j[i] ] > max)
			{
				max = p[i][ ind_j[i] ];
			}
		}
	}

	cout << ans << endl;
}

struct comparator_min {
 bool operator()(p_LL_i i, p_LL_i j) {
 return i.first > j.first;
 }
};

struct comparator_max {
 bool operator()(long long int i, long long int j) {
 return i <= j;
 }
};

// works!!!
// Let M = average number of words for each of the N separate word in query.
// O(MMlogN)
void search_snippets_4()
{
	int n; cin >> n;
	int m[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> m[i];
	}

	vector<vector<long long int> > p( n, vector<long long int>() );

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m[i]; ++j)
		{
			long long int p_ij; cin >> p_ij;

			p[i].push_back(p_ij);
		}
	}

	long long int min;
	long long int max;

	priority_queue< p_LL_i, vector<p_LL_i >, comparator_min> minHeap;
	priority_queue< long long int, vector<long long int>, comparator_max> maxHeap;

	int min_ind_i;

	for (int i = 0; i < n; ++i)
	{
		minHeap.push( make_pair(p[i][0], i) );
		maxHeap.push(p[i][0]);
	}

	min = minHeap.top().first;
	min_ind_i = minHeap.top().second;
	minHeap.pop();

	max = maxHeap.top();

	// Do sliding window now
	vector<int> ind_j(n, 0);

	long long int ans =  max - min + 1;

	while( ind_j[min_ind_i] < m[min_ind_i] )
	{
		if ( (max - min + 1) < ans )
		{
			ans = max - min + 1;
		}

		if (ind_j[min_ind_i] == m[min_ind_i]-1 )
		{
			break;
		}

		ind_j[min_ind_i]++;

		minHeap.push( make_pair(p[min_ind_i][ ind_j[min_ind_i] ], min_ind_i ) );

		maxHeap.push(p[min_ind_i][ ind_j[min_ind_i] ]);

		min = minHeap.top().first;
		min_ind_i = minHeap.top().second;
		minHeap.pop();

		max = maxHeap.top();
	}

	cout << ans << endl;
}

// wrong logic using binary search!!!
void search_snippets_5()
{
	int n; cin >> n;
	int m[n];
	int min_len_ind;
	long long int min_len = numeric_limits<long long int>::max();

	for (int i = 0; i < n; ++i)
	{
		cin >> m[i];

		if (m[i] < min_len)
		{
			min_len = m[i];

			min_len_ind = i;
		}
	}

	vector<vector<long long int> > p( n, vector<long long int>() );

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m[i]; ++j)
		{
			long long int p_ij; cin >> p_ij;

			p[i].push_back(p_ij);
		}
	}

	// for each element in the shortest list, do binary search over all elements for each of the other lists
	long long int ans = numeric_limits<long long int>::max();

	for (int j = 0; j < m[min_len_ind]; ++j)
	{
		long long int search = p[min_len_ind][j];

		long long int cur_ans;
		long long int cur_max = search;
		long long int cur_min = search;

		for (int i = 0; i < n; ++i)
		{
			if (i != min_len_ind)
			{
				// binary search for p[min_len_ind][j] in ith lists
				if (m[i]==1)
				{
					if (search < p[i][0])
					{
						if (p[i][0] > cur_max)
						{
							cur_max = p[i][0];
						}
					}
					else
					{
						if (p[i][0] < cur_min)
						{
							cur_min = p[i][0];
						}
					}

					continue;
				}

				int l = 0;
				int r = m[i]-1;

				while(l+1 != r)
				{
					int mid = (l+r)/2;
					if (p[i][mid] < search)
					{
						l = mid;
					}
					else
					{
						r = mid;
					}
				}

				if (search < p[i][l])
				{
					if (p[i][l] > cur_max)
					{
						cur_max = p[i][l];
					}
				}
				else if (search > p[i][r])
				{
					if (p[i][r] < cur_min)
					{
						cur_min = p[i][r];
					}
				}
				else
				{
					if ( (search-p[i][l]) < (p[i][r]-search) )
					{
						if (p[i][l] < cur_min)
						{
							cur_min = p[i][l];
						}
					}
					else if ((search-p[i][l]) > (p[i][r]-search))
					{
						if (p[i][r] > cur_max)
						{
							cur_max = p[i][r];
						}
					}
					/*else
					{
						if (cur_max == search)
						{
							
						}
					}*/
				}
			}
		}

		cur_ans = cur_max - cur_min + 1;

		cout << "\ncur_ans=" << cur_ans << endl;

		if (cur_ans < ans)
		{
			ans = cur_ans;
		}
	}
	
	cout << ans << endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		search_snippets_4();
	}

	return 0;
}