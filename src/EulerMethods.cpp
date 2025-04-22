#include "EulerMethods.h"
#include "parser.h"

#include <iostream>
#include <iomanip> // for std::setw and std::setprecision

void BasicEuler::solve(const std::string& equation, double x0, double y0, double h, int steps)
{
    EquationParser parser;
    parser.setAllowXY(true);
    parser.parseEquation(equation);

    std::cout << "\nBasic Euler Method:\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(6) << "Step" << std::setw(15) << "x" << std::setw(15) << "y\n";
    std::cout << "----------------------------------------\n";

    for (int i = 0; i < steps; ++i)
    {
        double f = parser.evaluate(x0, y0);
        y0 += h * f;
        x0 += h;
        std::cout << std::setw(6) << i + 1 << std::setw(15) << x0 << std::setw(15) << y0 << "\n";
    }
}

void ModifiedEuler::solve(const std::string& equation, double x0, double y0, double h, int steps)
{
    EquationParser parser;
    parser.setAllowXY(true);
    parser.parseEquation(equation);

    std::cout << "\nModified Euler Method:\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(6) << "Step" << std::setw(15) << "x" << std::setw(15) << "y\n";
    std::cout << "----------------------------------------\n";

    for (int i = 0; i < steps; ++i)
    {
        double k1 = parser.evaluate(x0, y0);
        double k2 = parser.evaluate(x0 + h, y0 + h * k1);
        y0 += h * (k1 + k2) / 2;
        x0 += h;
        std::cout << std::setw(6) << i + 1 << std::setw(15) << x0 << std::setw(15) << y0 << "\n";
    }
}
