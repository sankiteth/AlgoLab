#include <iostream>

using namespace std;

void approach_1()
{
	int n; cin >> n;

	int x[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> x[i];
	}

	int even_pairs = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = i; j < n; ++j)
		{

			int sum = 0;
			for (int k = i; k <= j; ++k)
			{
				sum += x[k];
			}

			if (sum%2 == 0)
			{
				even_pairs++;
			}
		}
	}

	cout << even_pairs << endl;
}

void approach_2()
{
	int n; cin >> n;

	int x[n], partial_sum[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> x[i];
	}

	partial_sum[0] = x[0];
	for (int i = 1; i < n; ++i)
	{
		partial_sum[i] = partial_sum[i-1] + x[i];
	}

	int even_pairs = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = i; j < n; ++j)
		{
			int sum;
			if (i == 0)
			{
				sum = partial_sum[j];
			}
			else
			{
				sum = partial_sum[j] - partial_sum[i-1];
			}

			if (sum % 2 == 0)
			{
				even_pairs++;
			}
		}
	}

	cout << even_pairs << endl;
}

void approach_3()
{
	int n; cin >> n;
	int x[n], partial_sum[n];

	for (int i = 0; i < n; ++i)
	{
		cin >> x[i];
	}

	partial_sum[0] = x[0];
	for (int i = 1; i < n; ++i)
	{
		partial_sum[i] = partial_sum[i-1] + x[i];
	}

	int even_pairs = 0, even = 0, odd = 0;

	for (int i = 0; i < n; ++i)
	{
		if (partial_sum[i]%2 == 0)
		{
			even++;
		}
		else
		{
			odd++;
		}
	}

	even_pairs += (even * (even-1)) / 2;
	even_pairs += (odd * (odd-1)) / 2;
	even_pairs += even;

	cout << even_pairs << endl;

}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		approach_3();
	}
	return 0;
}