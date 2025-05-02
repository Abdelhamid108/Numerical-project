#pragma once
#include <vector>

class PolynomialFitter {
public:
    // Construct with data points and desired degree
    PolynomialFitter(const std::vector<double>& x,
                     const std::vector<double>& y,
                     int degree);

    // Fit the polynomial (runs the normal-equation solver)
    void fit();

    // Retrieve coefficients a[0..degree]
    const std::vector<double>& coefficients() const;

private:
    int N, n;                              // N = # data points, n = degree
    std::vector<double> x, y, a;           // data & solution
    std::vector<std::vector<double>> B;    // augmented normal‐equation matrix

    void buildNormalMatrix();
    void gaussianElimination();
    void backSubstitution();
};
