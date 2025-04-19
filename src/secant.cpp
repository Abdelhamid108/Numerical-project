#include "secant.h"
#include "parser.h"
#include <iostream>
#include <cmath>

using namespace std;

SecantSolver::SecantSolver()   {
    get_input();
    solve();


}

void SecantSolver::get_input()
{
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
}

double SecantSolver::solve() {

    EquationParser Parser;  

    Parser.parseEquation(expr);
    double f0 = Parser.evaluate(x0);

    Parser.parseEquation(expr);
    double f1 = Parser.evaluate(x1);

    double x2;

    for (int i = 0; i < maxIter; ++i) {
        if (fabs(f1 - f0) < 1e-12) {
            cout << "Division by zero error in secant method." << endl;
            return x1;
        }

        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        
        Parser.parseEquation(expr);
        double f2 = Parser.evaluate(x2);

        cout << "Iteration " << i + 1 << ": x = " << x2 << ", f(x) = " << f2 << endl;

        if (fabs(x2 - x1) < tol) {
            cout << "Converged to root: " << x2 << endl;
            return x2;
        }

        x0 = x1;
        f0 = f1;
        x1 = x2;
        f1 = f2;
    }

    cout << "Did not converge within the maximum number of iterations. Last approximation: " << x2 << endl;
    return x2;
}
