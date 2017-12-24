#include <iostream>
#include <cmath>

using namespace std;

void seive(bool primes[], int n)
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
}

int main()
{
	int t; cin >> t;

	const int MAX = 41;
	bool primes[MAX];

	seive(primes, MAX);
	
	for (int i = 0; i < t; ++i)
	{
		int n; cin >> n;

		int ans = 1;

		for (int i = 2; i <= n; ++i)
		{
			if (primes[i])
			{
				int val = 1;

				while( val*i <= n)
				{
					val *= i;
				}

				ans *= val;
			}
		}

		cout << ans << endl;
	}

	return 0;
}