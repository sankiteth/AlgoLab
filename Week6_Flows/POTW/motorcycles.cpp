#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel IK;

//#include "../../../prettyprint.hpp"

void testcase() { 
    int n;
    cin >> n;
    // vector of tuples: y1, index, slope
    typedef tuple<double, int, CGAL::Quotient<double> > rider;
    vector<rider> riders;
    double a, x, y;
    for(int i=0;i<n;i++) {
        cin >> a >> x >> y;
        CGAL::Quotient<double> q(y-a,x);
        rider r(a, i, q);
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

int main(){
    int t;
    cin >> t;
    for(int i=0;i<t;i++) {
        testcase();
    }
    return 0;
}
