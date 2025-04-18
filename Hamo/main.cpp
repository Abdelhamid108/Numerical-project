#include <iostream>
#include "bisection.h"
#include "secant.h"

using namespace std;

int main() {
     string expr;
    double x0, x1, tol;
    int maxIter;



    cout << "Enter function f(x): ";
    getline(cin, expr);

    cout << "Enter first guess x0: ";
    cin >> x0;

    cout << "Enter second guess x1: ";
    cin >> x1;

    cout << "Enter tolerance: ";
    cin >> tol;

    cout << "Enter max iterations: ";
    cin >> maxIter;

    SecantSolver solver(expr);
    solver.solve(x0, x1, tol, maxIter);
   

    int t;
    
    cin>>t;
 
    return 0;
}
