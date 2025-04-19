#ifndef SECANT_H
#define SECANT_H

#include <string>

using namespace std;

class SecantSolver {
private:
    
    string expr;
    double x0, x1, tol;
    int maxIter;
public:
    SecantSolver();
    
    void get_input();

    double solve();
};

#endif // SECANT_H
