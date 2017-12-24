#include <iostream>

using namespace std;

void false_coin()
{
	int n, k;
	cin >> n; cin >> k;

	int light[n] = {0};
	int heavy[n] = {0};
	int normal[n] = {0};

	for (int weigh = 0; weigh < k; ++weigh)
	{
		int n_each_pan; cin >> n_each_pan;

		int coins[2*n_each_pan];
		for (int i = 0; i < 2*n_each_pan; ++i)
		{
			cin >> coins[i];
		}

		char result; cin >> result;

		if (result == '=')
		{
			for (int i = 0; i < 2*n_each_pan; ++i)
			{
				normal[ coins[i]-1 ] = 1;
			}
		}
		else if (result == '>')
		{
			for (int i = 0; i < n_each_pan; ++i)
			{
				heavy[ coins[i]-1 ] = 1;
			}

			for (int i = n_each_pan; i < 2*n_each_pan; ++i)
			{
				light[ coins[i]-1 ] = 1;
			}

			for (int i = 0; i < n; ++i)
			{
				if (!normal[i])
				{
					int flag = 1;

					for (int j = 0; j < 2*n_each_pan; ++j)
					{
						if ( i == (coins[j]-1) )
						{
							flag = 0;
							break;
						}
					}

					if (flag)
					{
						normal[i] = 1;
					}
				}
			}
		}
		else if (result == '<')
		{
			for (int i = 0; i < n_each_pan; ++i)
			{
				light[ coins[i]-1 ] = 1;
			}

			for (int i = n_each_pan; i < 2*n_each_pan; ++i)
			{
				heavy[ coins[i]-1 ] = 1;
			}

			for (int i = 0; i < n; ++i)
			{
				if (!normal[i])
				{
					int flag = 1;

					for (int j = 0; j < 2*n_each_pan; ++j)
					{
						if ( i == (coins[j]-1) )
						{
							flag = 0;
							break;
						}
					}

					if (flag)
					{
						normal[i] = 1;
					}
				}
			}
		}
	}

	int res, count = 0;
	for (int i = 0; i < n; ++i)
	{
		if ( normal[i] || (light[i] && heavy[i]) )
		{
			continue;
		}

		res = i;
		count++;
	}

	if (count > 1)
	{
		cout << 0 << endl;
	}
	else
	{
		cout << (res+1) << endl;
	}
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		false_coin();
	}
	return 0;
}