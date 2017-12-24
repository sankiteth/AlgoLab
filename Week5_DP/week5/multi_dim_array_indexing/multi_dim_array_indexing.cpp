#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

void func()
{
	int k,n,q; cin >> k >> n >> q;
	vector<pair<int, int> > bounds(k);
	for (int i = 0; i < k; ++i)
	{
		int a_i,b_i; cin >> a_i >> b_i;
		bounds[i] = make_pair(a_i, b_i);
	}

	//map<vector<int>, int> notebook;
	unordered_map <vector<int>, int, boost::hash<std::vector<int> > > notebook;

	for (int i = 0; i < n; ++i)
	{
		vector<int> exp(k);
		for (int j = 0; j < k; ++j)
		{
			cin >> exp[j];
		}

		int r; cin >> r;

		notebook[exp] = r;
	}

	for (int i = 0; i < q; ++i)
	{
		vector<int> query(k);
		for (int j = 0; j < k; ++j)
		{
			cin >> query[j];
		}

		if (notebook.find(query) != notebook.end())
		{
			cout << notebook[query] << endl;
		}
		else
		{
			cout << -1 << endl;
		}
	}

}

int main()
{
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		func();
	}

	return 0;
}