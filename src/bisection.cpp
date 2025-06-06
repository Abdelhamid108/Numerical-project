#include "bisection.h"
#include "parser.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

bisection::bisection()
{
    get_input();
    bisection_solve();

}

void bisection::get_input()
{
    cout << "Enter function f(x): ";
    getline(cin, expr);

    cout << "Enter first guess x0: ";
    cin >> a;

    cout << "Enter second guess x1: ";
    cin >> b;

    cout << "Enter tolerance: ";
    cin >> tol;

    cout << "Enter max iterations: ";
    cin >> maxIter;
}

void bisection::bisection_solve()
{

    EquationParser Parser;

    Parser.parseEquation(expr);
    double fa = Parser.evaluate(a);

    Parser.parseEquation(expr);
    double fb = Parser.evaluate(b);

    if (fa * fb >= 0) {
        cout << "No sign change: f(a) and f(b) must have opposite signs.\n";
        return;
    }

    cout << left << setw(8) << "Iter" 
         << setw(15) << "a" 
         << setw(15) << "b" 
         << setw(15) << "c" 
         << setw(15) << "f(c)" << endl;

    for (int i = 1; i <= maxIter; ++i) {
        double c = (a + b) / 2;
        Parser.parseEquation(expr);
        double fc = Parser.evaluate(c);

        cout << left << setw(8) << i
             << setw(15) << a
             << setw(15) << b
             << setw(15) << c
             << setw(15) << fc << endl;

        if (fabs(fc) < tol) {
            cout << "\nRoot found: " << c << "\n";
            return;
        }

        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }

    cout << "\nApproximate root after max iterations: " << (a + b) / 2 << "\n";
}
