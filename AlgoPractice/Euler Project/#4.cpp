#include <iostream>
#include <string>
#include <set>

using namespace std;

bool isPalin(int n)
{
    string num = to_string(n);
    for(int i = 0; i < num.size() / 2; ++i)
        if(num[i] != num[num.size()-1-i])
            return false;
    return true;
}

void generatePalindromes(set<int> &s)
{
    for(int i = 100; i < 1000; ++i)
    	// atleast 6 digits constraint
        for(int j = 100000/i; j < 1000; ++j)
            if(isPalin(i*j))
            {
                s.insert(i*j);
            }
}

int main()
{
	int t; cin >> t;
	set<int> s;
	generatePalindromes(s);

	for (int i = 0; i < t; ++i)
	{
		long int n; cin >> n;

		cout << *(--s.lower_bound(n)) << endl;
	}

	return 0;
}