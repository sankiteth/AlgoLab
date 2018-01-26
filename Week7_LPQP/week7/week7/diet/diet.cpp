#include <iostream>
#include <cassert>
#include <cmath>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int floor_to_double(const SolT& x)
  {
    double a = std::floor(CGAL::to_double(x));

    // make sure by actual comparison
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;

    return (int)a; 
  }

  // n = number of nutrients
  // m = number of foods
  void testcase(int n, int m) {
    int min[n], max[n];
    for (int i=0; i < n; ++i){
      std::cin >> min[i] >> max[i];
    }

    int p[m]; // price per unit of mth food 
    int C[m][n]; // m types of food; n types of nutrients in each food
    for (int i=0; i<m; ++i){
      std::cin >> p[i];
      for (int j=0; j<n; ++j){
        std::cin >> C[i][j];
      }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0); // set lower bounds to 0, upper bounds to infinity

    int X[m];
    for(int i=0; i<m; ++i){
      X[i] = i;
    }

    // 2n constraints, 2 for each nutrient type
    for(int i=0; i<n; i++){
      for(int j=0; j<m; j++){
        lp.set_a(X[j], i, -1 * C[j][i]); 
        lp.set_a(X[j], n + i, C[j][i]);
      }

      lp.set_b(i  , -1 * min[i]);
      lp.set_b(n+i,      max[i]);
    }

    // objective function
    for(int i=0; i<m; i++){
      lp.set_c(X[i], p[i]);
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    if (s.is_optimal()){
      double val = floor_to_double(s.objective_value());
      std::cout << val << std::endl;
    } else {
      std::cout << "No such diet." << std::endl;
    }
  }

int main() {
  int n, m; 
  std::cin >> n >> m;
  while(n!=0 && m!=0) {
    testcase(n, m);
    std::cin >> n >> m;
  }
}