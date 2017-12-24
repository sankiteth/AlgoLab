#include <iostream>
#include <cmath>
#include <utility> 

using namespace std;

// we need pth prime number. Searching for it in 2 to n.
pair<int, long long int> regular_seive(bool primes[], int n, int p)
{
	for (int i = 0; i < n; ++i)
	{
		primes[i] = true;
	}

	for (int i = 2; i <= sqrt(n); ++i)
	{
		if (primes[i] == true)
		{
			for (int j = i*i; j <= n; j=j+i)
			{
				primes[j] = false;
			}
		}
	}

	int num_primes = 0;
	long long int ans = 0;

	for (int i = 2; i < n; ++i)
	{
		if (primes[i])
		{
			num_primes++;

			if (num_primes == p)
			{
				ans = i;
				break;
			}
		}
	}

	return make_pair(num_primes, ans);
}

int seive(long long int MAX, int p)
{
	long long int base = sqrt(MAX); // each bin size is sqrt(MAX)

	bool base_primes[base+1];

	pair<int, long long int> ret = regular_seive(base_primes, base+1, p);

	int num_primes = ret.first;
	long long int ans = ret.second;

	// The required prime is in first bin itself.
	if (ans != 0)
	{
		return ans;
	}

	for (long long int i = base+1; i <= MAX; i=i+base )
	{
		bool primes[base];

		for (long long int j = 0; j < base; ++j)
		{
			primes[j] = true;
		}

		for (long long int j = 2; j < base+1; ++j)
		{
			if (base_primes[j] == true) // if a base prime
			{
				for (long long int k = int(ceil( i/(j*1.0) )) * j ; k < i+base; k=k+j)
				{
					primes[k-i] = false;
				}
			}
		}

		// count the primes in the current bin
		for (long long int j = 0; j < base; ++j)
		{
			if (primes[j])
			{
				num_primes++;
				if (num_primes == p)
				{
					ans = i+j;
					return ans;
				}
			}
		}
	}

	return 0;
}

int main()
{
	int t; cin >> t;

	const long long int MAX = 104730; // 1<=p<=10,000. Assuming the pth prime is always less than MAX.

	for (int i = 0; i < t; ++i)
	{
		int p; cin >> p;
		
		int ans = seive(MAX, p);

		cout << ans << endl;
	}

	return 0;
}