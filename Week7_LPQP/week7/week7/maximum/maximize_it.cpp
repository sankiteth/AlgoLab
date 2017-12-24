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


double ceil_to_double(const SolT& x)
  {
    double a = std::ceil(CGAL::to_double(x));

    // make sure by actual comparison
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;

    return a;
    
  }

double floor_to_double(const SolT& x)
  {
    double a = std::floor(CGAL::to_double(x));

    // make sure by actual comparison
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;

    return a;
    
  }  

void testcase(int p, int a, int b) {
  //std::cout << a << " " << b << " " << std::endl;

  Program qp_max (CGAL::SMALLER, true, 0, false, 0); // set lower bounds to 0, upper bounds to infinity
  Program qp_min (CGAL::LARGER, false, 0, true, 0);  // set lower bounds to minus infinity, upper bounds to zero

  // now set the non-default entries
  const int X = 0;
  const int Y = 1;
  const int Z = 2;

  qp_max.set_a(X, 0,  1); qp_max.set_a(Y, 0, 1); qp_max.set_b(0, 4);    //  x + y  <= 4
  qp_max.set_a(X, 1,  4); qp_max.set_a(Y, 1, 2); qp_max.set_b(1, a*b);  // 4x + 2y <= ab
  qp_max.set_a(X, 2, -1); qp_max.set_a(Y, 2, 1); qp_max.set_b(2, 1);    // -x +  y <= 1
  
  // min ax^2 - by
  qp_max.set_d(X, X, 2*a);
  qp_max.set_c(Y, -b);

  qp_min.set_a(X, 0,  1); qp_min.set_a(Y, 0, 1);                        qp_min.set_b(0, -4);    //  x +  y     >= -4
  qp_min.set_a(X, 1,  4); qp_min.set_a(Y, 1, 2); qp_min.set_a(Z, 1, 1); qp_min.set_b(1, -a*b);  // 4x + 2y + z >= -ab
  qp_min.set_a(X, 2, -1); qp_min.set_a(Y, 2, 1);                        qp_min.set_b(2, -1);     // -x +  y     >= -1
  qp_min.set_l(Z, true, 0); // set lower bound to 0
  qp_min.set_u(Z, false, 0); // set upper bound to infinity

  // min ax^2 + by + z^2
  qp_min.set_d(X, X, 2*a);
  qp_min.set_d(Z, Z, 2*1);
  qp_min.set_c(Y, b);

  // solve the program, using ET as the exact type
  Solution s;
  if (p == 1) {
    s = solve_quadratic_program(qp_max, ET());
    assert (s.solves_quadratic_program(qp_max));
  } else {
    s = solve_quadratic_program(qp_min, ET());
    assert (s.solves_quadratic_program(qp_min));
  }

  if (s.status() == CGAL::QP_INFEASIBLE) {
    std::cout << "no" << std::endl;
  } else if (s.status() == CGAL::QP_UNBOUNDED) {
    std::cout << "unbounded" << std::endl;
  } else {
    assert (s.status() == CGAL::QP_OPTIMAL);
    double val;
    if (p == 1) {
      //std::cout << s.objective_value() << std::endl;
      val = floor_to_double(-1 * s.objective_value());
    } else {
      val = ceil_to_double(s.objective_value());
    } 

    std::cout << (int)val << std::endl;
  }
}

int main() {
  int p, a, b;
  std::cin >> p >> a >> b;
  while(p != 0) {
    testcase(p, a, b);
    std::cin >> p >> a >> b;
  }
  return 0;
}
