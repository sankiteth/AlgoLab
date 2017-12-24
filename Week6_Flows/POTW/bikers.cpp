#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <list>
#include <utility>

using namespace std;

typedef long long LLI;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef tuple<K::FT, CGAL::Quotient<K::FT>, int> bike;

bool ith_bike_wins(R& r1, R& r2)
{
	auto o = CGAL::intersection(r1, r2);
	const P* op = boost::get<P>(&*o);
	// sources are always on y axis, hence y values can be used to compare distances
	K::FT d1 = CGAL::abs(r1.source().y() - (*op).y()); //CGAL::squared_distance(bikes[i].first.source(), *op);
	K::FT d2 = CGAL::abs(r2.source().y() - (*op).y()); //CGAL::squared_distance(bikes[*it].first.source(), *op);

	return d1 < d2;
}

void bikers_nlogn()
{
	int n; cin >> n;
	vector<bike> bikes(n);
	for (int i = 0; i < n; ++i)
	{
		double y0,x1,y1; cin >> y0 >> x1 >> y1;
		K::FT Y0(y0), X1(x1), Y1(y1);

		bike b(Y0, CGAL::Quotient<K::FT>(y1-y0, x1), i);
		bikes[i] = b;
	}

	sort(bikes.begin(), bikes.end(), [](bike a, bike b)->bool
										{
											return get<0>(a) < get<0>(b);
										}
		);

	vector<int> dp(n);
	vector<int> indexes(n, -1);
	dp[0] = 0;
	indexes[0] = get<2>(bikes[0]);
	int left = 0;
	int right = 0;
	
	for (int i = 1; i < n; ++i)
	{	
		if (get<1>(bikes[i]) >= get<1>(bikes[dp[right]]) )
		{
			//cout << "\nNo Intersect1: " << i << " " << dp[right];
			right++;
			dp[right] = i;
			indexes[right] = get<2>(bikes[i]);
		}
		else if (CGAL::abs( get<1>(bikes[i]) ) >= CGAL::abs( get<1>(bikes[dp[right]]) ))
		{
			//cout << "\nBike stops1: " << i << " " << dp[right];
			continue;
		}
		else
		{
			bool flag = true;
			int l = -1;
			int r = right;
			int mid;
			while(l+1 < r)
			{
				mid = (l+r)/2;
				//cout << "\nmid=" << mid;
				if (get<1>(bikes[i]) >= get<1>(bikes[dp[mid]]))
				{
					//cout << "\nNo Intersect2: " << i << " " << dp[mid];
					l = mid;
				}
				//else if (!ith_bike_continues(bikes[i], bikes[dp[mid]]))
				else if (CGAL::abs( get<1>(bikes[i]) ) >= CGAL::abs( get<1>(bikes[dp[mid]]) ))
				{
					flag = false;
					//cout << "\nBike stops2: " << i << " " << dp[mid];
        			l = mid;
				}
				else
				{
					//cout << "\nWin: " << i << " " << dp[mid];
					r = mid;
				}
			}

			if (flag)
			{
				// ith bike becomes part of solution
				dp[r] = i;
				indexes[r] = get<2>(bikes[i]);
				right = r;
			}
			else
			{
				// ith bike is not part of solution, but it removes some bikes from solution
				right = r-1;
			}
		}

		/*cout << "\ncurrent: left=" << left << " right=" << right << endl;
		for (int j = 0; j < n; ++j)
		{
			cout << indexes[j] << " ";
		}
		cout << endl;
		for (int j = 0; j < n; ++j)
		{
			cout << dp[j] << " ";
		}
		cout << endl;*/
	}

	sort(indexes.begin()+left, indexes.begin()+right+1);

	for (int i = left; i <= right; ++i)
	{
		cout << indexes[i] << " ";
	}

	cout << endl;
}

void bikers_n2()
{
	int n; cin >> n;
	vector<pair<R, int> > bikes(n);
	for (int i = 0; i < n; ++i)
	{
		LLI y0,x1,y1; cin >> y0 >> x1 >> y1;
		P s(0,y0);
		P dir(x1,y1);
		bikes[i] = make_pair(R(s,dir), i);
	}

	sort(bikes.begin(), bikes.end(), [](pair<R,int> a, pair<R,int> b)->bool
										{
											return a.first.source().y() < b.first.source().y();
										}
		);

	list<int> dp;
	vector<int> indexed(n, 0);
	dp.push_front(0);
	indexed[bikes[0].second] = 1;
	for (int i = 1; i < n; ++i)
	{
		list<int>::iterator it = dp.begin();
		bool flag = true;
		while(it != dp.end())
		{
			if (!CGAL::do_intersect(bikes[i].first, bikes[*it].first))
			{
				//cout << "\nNo Intersect1: " << i << " " << *it;
				break;
			}
			else if (!ith_bike_wins(bikes[i].first, bikes[*it].first))
        	{
        		//cout << "\nNo Win1: " << i << " " << *it;
        		// the ith bike cannot be part of solution
        		flag = false;
        		break;
        	}
        	else
        	{
        		//cout << "\nWin1: " << i << " " << *it;
        		indexed[bikes[*it].second] = 0;
        		it = dp.erase(it);
        	}
		}

		if (flag)
		{
			//cout << "\nAll others stop except bike i";
			indexed[bikes[i].second] = 1;
			dp.push_front(i);
		}
	}

	for (int i = i; i < n; ++i)
	{
		if (indexed[i] == 1)
			cout << i << " ";
	}

	cout << endl;
}

void testcase() { 
    int n;
    cin >> n;
    // vector of tuples: y1, index, slope
    typedef tuple<K::FT, int, CGAL::Quotient<K::FT> > rider;
    vector<rider> riders;
    double a, x, y;
    for(int i=0;i<n;i++) {
        cin >> a >> x >> y;

        K::FT A(a), X(x), Y(y);
        CGAL::Quotient<K::FT> q(y-a,x);
        rider r(A, i, q);
        riders.push_back(r);
    }

    sort(riders.begin(), riders.end()); //ascending
    
    vector<rider> candidates;
    candidates.push_back(riders[0]); 

    for(int i=1;i<n;i++) {
        while(true) {
            // if stack is empty, put in rider i
            if(candidates.empty()){
                candidates.push_back(riders[i]);
                break;
            }
            // if slope is bigger than top stack element, add new elem to stack
            if(get<2>(riders[i]) >= get<2>(candidates.back())) {
                candidates.push_back(riders[i]);
                break;
            }
            if(CGAL::abs( get<2>(riders[i]) ) >= CGAL::abs( get<2>(candidates.back()) )) {
                //the new rider i stops
                break;
            } else {
                //rider i keeps going, the top element of the stack gets removed
                candidates.pop_back();
            }
        }
    }

    // order the riders by index now
    vector<int> out_indices;
    for(int i=0;i<candidates.size();i++) { 
        out_indices.push_back(get<1>(candidates[i]));
    }
    sort(out_indices.begin(), out_indices.end());
    for(int i=0;i<out_indices.size();i++) { 
        cout << out_indices[i] << " ";
    }
    cout << endl;
        
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		bikers_n2();
	}

	return 0;
}