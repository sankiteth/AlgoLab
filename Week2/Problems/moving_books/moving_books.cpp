#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <set>
#include <list>

using namespace std;

void move_books_mnm()
{
	int n, m; cin >> n >> m;

	int s[n], w[m];

	for (int i = 0; i < n; ++i)
	{
		cin >> s[i];
	}

	sort(s,s+n, greater<int>());

	for (int i = 0; i < m; ++i)
	{
		cin >> w[i];
	}

	int box_left = m;
	int time = 0;

	while(box_left > 0)
	{
		bool at_least_one_lifted = false;

		int box_left_till_now = box_left;

		for (int i = 0; i < min(n, box_left_till_now); ++i) // for each friend
		{
			int diff = numeric_limits<int>::max();
			int index = -1;

			for (int j = 0; j < m; ++j) // for each box
			{
				if (w[j] != 0 && w[j] <= s[i]) // box still not moved.
				{
					if ( (s[i]-w[j]) < diff )
					{
						diff = s[i]-w[j];
						index = j;
						at_least_one_lifted = true;
					}

					if (diff == 0)
					{
						break;
					}
				}
			}

			if (index != -1)
			{
				box_left--;
				w[index] = 0;
			}

			/*cout << endl;
			for (int j = 0; j < m; ++j)
			{
				cout << w[j] << " ";
			}
			cout << "box_left=" << box_left << endl;*/
		}

		if (!at_least_one_lifted)
		{
			cout << "impossible" << endl;
			return;
		}

		time += 2;

		// return
		if (box_left > 0)
		{
			time += 1;
		}

		/*char c;
		cin >> c;*/

	}

	cout << time << endl;
}

void move_books_mnlogm()
{
	int n, m; cin >> n >> m;

	int s[n];
	multiset<int> w;

	for (int i = 0; i < n; ++i)
	{
		cin >> s[i];
	}

	sort(s,s+n, greater<int>());

	for (int i = 0; i < m; ++i)
	{
		int w_i;
		cin >> w_i;
		w.insert(w_i);
	}

	int box_left = m;
	int time = 0;

	while(box_left > 0)
	{
		/*cout << endl;
		for (multiset<int>::iterator w_it = w.begin(); w_it != w.end(); ++w_it)
		{
			cout << *w_it << " ";
		}
		cout << "box_left=" << box_left << endl;*/

		bool at_least_one_lifted = false;

		int box_left_till_now = box_left;

		for (int i = 0; i < min(n, box_left_till_now); ++i) // for each friend
		{
			multiset<int>::iterator w_it = w.lower_bound(s[i]);

			//cout << "\n*w_it=" << *w_it << endl;

			// w.begin() returned if first item matches search term or search term less than smallest element.
			if( (w_it == w.begin() && *w_it == s[i]) || (w_it != w.begin()) )
			{
				// w.end() returned when search term greater than all elements.
				// the exact pointer returned if match found.
				// if match not found, the next largest element to search term is returned.
				if( (w_it == w.end()) || (*w_it > s[i]) )
				{
			    	--w_it;
				}

			    w.erase(w_it);
			    box_left--;
			    at_least_one_lifted = true;
			}
		}

		if (!at_least_one_lifted)
		{
			cout << "impossible" << endl;
			return;
		}

		time += 2;

		// return
		if (box_left > 0)
		{
			time += 1;
		}

		/*char c;
		cin >> c;*/

	}

	cout << time << endl;
}

void move_books_nm()
{
	int n, m; cin >> n >> m;

	int s[n];
	list<int> w;

	for (int i = 0; i < n; ++i)
	{
		cin >> s[i];
	}

	sort(s, s+n, greater<int>());

	for (int i = 0; i < m; ++i)
	{
		int w_i;
		cin >> w_i;
		w.push_front(w_i);
	}

	w.sort(greater<int>());

	if ( *(w.begin()) > s[0] )
	{
		cout << "impossible" << endl;
		return;
	}

	int box_left = m;
	int time = 0;

	while(box_left > 0)
	{
		/*cout << endl;
		for (list<int>::iterator w_it = w.begin(); w_it != w.end(); ++w_it)
		{
			cout << *w_it << " ";
		}
		cout << "box_left=" << box_left << endl;*/

		int s_it = 0;
		list<int>::iterator w_it = w.begin();
		list<int>::iterator w_it_remove = w.begin();

		int size = min(n, box_left);

		while( s_it < size && w_it != w.end() )
		{
			if (s[s_it] >= *w_it)
			{
				w_it_remove = w_it;
				w_it++;

				w.erase(w_it_remove);
				box_left--;

				s_it++;
			}
			else
			{
				w_it++;
			}
		}

		time += 2;

		// return
		if (box_left > 0)
		{
			time += 1;
		}

		/*char c;
		cin >> c;*/

	}

	cout << time << endl;
}

bool is_feasible_sol(int s[], int w[], int m, int k)
{
	int ind_s = 0;
	int ind_w = 0;

	while(ind_w < m)
	{
		// if k is min number of trips, then the first k weights, after reverse resorting, should be less than the 
		// first friend's strength, after reverse sorting.
		if ( (ind_w < m) && (s[ind_s] < w[ind_w]) )
		{
			return false;
		}

		ind_w += k;
		ind_s += 1;
	}

	return true;
}

void move_books_mlogm()
{
	int n, m; cin >> n >> m; // n= num of friends, m= num of books

	int s[n];
	int w[m];

	for (int i = 0; i < n; ++i)
	{
		cin >> s[i];
	}

	sort(s, s+n, greater<int>());

	for (int i = 0; i < m; ++i)
	{
		cin >> w[i];
	}

	sort(w, w+m, greater<int>());

	if ( w[0] > s[0] )
	{
		cout << "impossible" << endl;
		return;
	}

	int l = m/(n*1.0);
	int r = m;
	int mid = l;

	int min_trips = -10;

	// Binary search for the minimum number of trips
	while(l <= r)
	{
		mid = (l+r)/2;

		if (is_feasible_sol(s, w, m, mid))
		{
			min_trips = mid;
			r = mid-1;
		}
		else
		{
			l = mid+1;
		}
	}

	/*
	// No binary search
	for (int i = 1; i <= m; ++i)
	{
		if (is_feasible_sol(s, w, m, i))
		{
			min_trips = i;
			break;
		}
	}*/

	cout << (min_trips-1)*3 + 2 << endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		move_books_mlogm();
	}

	return 0;
}
