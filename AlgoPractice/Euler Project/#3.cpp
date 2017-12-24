#include <iostream>
#include <math.h>

using namespace std;

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		long long int n; cin >> n;
		long long int ans = 0;
		
		while(n%2 == 0)
		{
			n = n/2;
		}

		if (n == 1)
		{
			cout << 2 << endl;
		}
		
		while(n > 1)
		{
			bool is_prime = true;
			for (int i = 3; i <= sqrt(n); i=i+2)
			{
				if (n%i == 0)
				{
					is_prime = false;
					n = n/i;
					break;
				}
			}

			if (is_prime)
			{
				ans = n;
				break;
			}
		}

		cout << ans << endl;
	}

	return 0;
}