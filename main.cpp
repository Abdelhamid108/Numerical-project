#include <iostream>
#include <vector>
#include <iomanip>
#include "PolynomialFitter.h"

int main() {
    std::cout << std::fixed << std::setprecision(4);

    int N;
    std::cout << "Enter number of data points: ";
    std::cin >> N;

    std::vector<double> x(N), y(N);
    std::cout << "Enter x values:\n";
    for (auto& xi : x) std::cin >> xi;
    std::cout << "Enter y values:\n";
    for (auto& yi : y) std::cin >> yi;

    int degree;
    std::cout << "Enter polynomial degree: ";
    std::cin >> degree;

    try {
        PolynomialFitter fitter(x, y, degree);
        fitter.fit();
        const auto& a = fitter.coefficients();

        std::cout << "\nFitted polynomial:\ny = ";
        for (int i = 0; i <= degree; ++i) {
            if (i > 0) std::cout << " + ";
            std::cout << "(" << a[i] << ")x^" << i;
        }
        std::cout << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
