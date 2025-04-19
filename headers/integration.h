#ifndef NUMERICAL_INTEGRATOR_H
#define NUMERICAL_INTEGRATOR_H

#include "parser.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>

class NumericalIntegrator {
private:
    EquationParser parser;
    double a, b;
    int n;
    std::vector<double> x, fx;
    double h;

    double getBoundInput(const std::string& prompt);
    void getValidInput(const std::string& prompt, int& value, int min_val);
    void generatePoints();
    void displayTable();
    double trapezoidalRule();
    double simpsons13Rule();
    double simpsons38Rule();

public:
    NumericalIntegrator();
};

#endif // NUMERICAL_INTEGRATOR_H