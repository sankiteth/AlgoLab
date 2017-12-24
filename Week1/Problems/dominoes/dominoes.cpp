#include <iostream>

using namespace std;

void dominoes()
{
	int n; cin >> n;
	
	int h[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> h[i];
	}

	int fallen = 0;

	// The leftmost always falls
	fallen++;

	// Effect carried over to next domino after current dommino falls.
	int left_over = h[0]-1;

	for (int i = 1; i < n; ++i)
	{
		if (left_over==0)
		{
			break;
		}

		fallen++;

		if (h[i] > left_over)
		{
			left_over = h[i] - 1;
		}
		else
		{
			left_over -= 1;
		}
	}

	cout << fallen << endl;
}

int main()
{
	//std::ios_base::sync_with_stdio(false);

	int t; cin >> t;

	for (int i = 0; i < t; ++i)
	{
		dominoes();
	}

	return 0;
}