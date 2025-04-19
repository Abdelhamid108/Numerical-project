#ifndef DIVIDED_DIFFERENCE_INTERPOLATOR_H
#define DIVIDED_DIFFERENCE_INTERPOLATOR_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>

class divide {
public:
    void askP();      // asks for number of points
    void askX();      // asks for x values
    void askF();      // asks for y values
    void askXX();     // asks for x at which you want to calculate y
    void diffTable(); // calculate and display table
    void calcP();     // calculate and display the desired y
    divide();

private:
    double XX, x[20], f[20][20], P1;
    int n;
};

// Helper function declaration
double getValidatedDouble(std::string prompt);

#endif // DIVIDED_DIFFERENCE_INTERPOLATOR_H