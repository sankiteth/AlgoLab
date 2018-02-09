#include <iostream>
#include <cmath>

using namespace std;

void deck_of_cards_1()
{
	int n, k; cin >> n >> k;
	int x[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> x[i];
	}

	int partial_sums[n];
	partial_sums[0] = x[0];

	for (int i = 1; i < n; ++i)
	{
		partial_sums[i] = partial_sums[i-1] + x[i];
	}

	int val = abs(k - partial_sums[0]);

	int left = 0, right = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = i; j < n; ++j)
		{
			int sum;
			if (i == 0)
			{
				sum = partial_sums[j];
			}
			else
			{
				sum = partial_sums[j] - partial_sums[i-1];
			}

			if (val > abs(k - sum))
			{
				left = i;
				right = j;
				val = abs(k - sum);
			}

			if (val == 0)
			{
				cout << left << " " << right << endl;
				return;
			}
		}
	}

	cout << left << " " << right << endl;
}

void deck_of_cards_2()
{
	int n, k; cin >> n >> k;

	// all elements non-negetive, hence sliding window works!
	int x[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> x[i];
	}

	int best_l = 0, best_r = 0;
	int cur_l = 0, cur_r = 0;
	int best_sum = x[0], cur_sum = x[0];

	if (cur_sum == k)
	{
		cout << best_l << " " << best_r << endl;
		return;
	}

	while(cur_r < n)
	{
		if (cur_sum < k)
		{
			cur_r++;
			cur_sum += x[cur_r];
		}
		else if (cur_sum > k)
		{
			cur_sum -= x[cur_l];
			cur_l++;

			// when left pointer exceeds right pointer, both made to point to the current element.
			if (cur_l > cur_r)
			{
				cur_r = cur_l;
				cur_sum = x[cur_l];
			}
		}

		if (cur_sum == k)
		{
			best_l = cur_l; 
			best_r = cur_r;
			break;

		}

		if (abs(k - cur_sum) < abs(k - best_sum)  )
		{
			best_sum = cur_sum;
			best_l = cur_l; 
			best_r = cur_r;
		}
	}

	cout << best_l << " " << best_r << endl;
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		deck_of_cards_2();
	}

	return 0;
}