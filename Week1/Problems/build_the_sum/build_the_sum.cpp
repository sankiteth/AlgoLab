#include <iostream>

using namespace std;

void build_the_sum()
{
	int m; cin >> m;

	double x[m];
	for (int i = 0; i < m; ++i)
	{
		cin >> x[i];
	}

	double sum = 0.0;
	for (int i = 0; i < m; ++i)
	{
		sum += x[i];
	}

	cout << sum << endl;
}

int main()
{
	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		build_the_sum();
	}
}