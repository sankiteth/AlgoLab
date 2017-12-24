#include <iostream>

using namespace std;

int main()
{
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		long long int n; cin >> n;

		long long int n_3 = (n-1)/3;
		long long int n_5 = (n-1)/5;
		long long int n_15 = (n-1)/15;

		// numbers multiple of both 3 and 5 (15) counted twice!
		long long int sum = ( (3*n_3*(n_3+1) ) + (5*n_5*(n_5+1) ) - (15 *n_15*(n_15+1)) ) / 2;

		cout << sum << endl;
	}

	return 0;
}