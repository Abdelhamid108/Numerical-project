#include "secant.h"
#include "parser.h"
#include <iostream>
#include <cmath>

using namespace std;

SecantSolver::SecantSolver(const string& expr) : expression(expr) {}

double SecantSolver::solve(double x0, double x1, double tolerance, int maxIterations) {

    EquationParser Parser;  

    Parser.parseEquation(expression);
    double f0 = Parser.evaluate(x0);

    Parser.parseEquation(expression);
    double f1 = Parser.evaluate(x1);

    double x2;

    for (int i = 0; i < maxIterations; ++i) {
        if (fabs(f1 - f0) < 1e-12) {
            cout << "Division by zero error in secant method." << endl;
            return x1;
        }

        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        
        Parser.parseEquation(expression);
        double f2 = Parser.evaluate(x2);

        cout << "Iteration " << i + 1 << ": x = " << x2 << ", f(x) = " << f2 << endl;

        if (fabs(x2 - x1) < tolerance) {
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
