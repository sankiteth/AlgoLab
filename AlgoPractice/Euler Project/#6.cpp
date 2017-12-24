#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		long long int n; cin >> n;

		long long int s1 = (n*(n+1)/2) * (n*(n+1)/2);
		long long int s2 = n*(n+1)*(2*n + 1)/6;

		cout << abs(s1-s2) << endl;
	}

	return 0;
}