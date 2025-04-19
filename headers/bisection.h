#ifndef BISECTION_H
#define BISECTION_H

#include <string>
using namespace std;
class bisection{
    private:
        string expr;
        double a, b, tol;
        int maxIter;
    public:
        bisection();
        void get_input();
        void bisection_solve();
};
#endif // BISECTION_H
