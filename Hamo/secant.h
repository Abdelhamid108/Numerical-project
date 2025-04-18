#ifndef SECANT_H
#define SECANT_H

#include <string>

using namespace std;

class SecantSolver {
private:
    string expression;

public:
    SecantSolver(const string& expr);

    double solve(double x0, double x1, double tolerance = 1e-6, int maxIterations = 100);
};

#endif // SECANT_H
