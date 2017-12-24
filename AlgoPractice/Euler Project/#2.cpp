#include <iostream>

using namespace std;

int main()
{
	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		long long int n; cin >> n;

		long long int f1 = 2;
		long long int f2 = 8;

		long long int sum = 10;

		long long int f = 4*f2 + f1;

		while(f<=n)
		{
			sum += f;

			f1 = f2;
			f2 = f;

			f = 4*f2 + f1;
		}

		cout << sum << endl;
	}

	return 0;
}