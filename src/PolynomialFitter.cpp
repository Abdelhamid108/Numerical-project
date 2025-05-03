#include "PolynomialFitter.h"
#include <cmath>
#include <stdexcept>

// constructor: copy data & allocate matrix
PolynomialFitter::PolynomialFitter(const std::vector<double>& xv,
                                   const std::vector<double>& yv,
                                   int degree)
 : N(int(xv.size())), n(degree),
   x(xv), y(yv),
   a(degree + 1),
   B(degree + 1, std::vector<double>(degree + 2))
{
    if (N == 0 || degree < 0 || N <= degree)
        throw std::invalid_argument("Need more points than degree");
}

void PolynomialFitter::fit() {
    buildNormalMatrix();
    gaussianElimination();
    backSubstitution();
}

const std::vector<double>&
PolynomialFitter::coefficients() const {
    return a;
}

void PolynomialFitter::buildNormalMatrix() {
    // compute all sums Σ x^k for k = 0..2n
    std::vector<double> X(2*n + 1, 0.0);
    for (int k = 0; k < 2*n + 1; ++k)
        for (int i = 0; i < N; ++i)
            X[k] += std::pow(x[i], k);

    // fill normal‐eqns coefficients
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= n; ++j)
            B[i][j] = X[i + j];

    // compute RHS sums Σ x^k * y for k = 0..n
    for (int i = 0; i <= n; ++i) {
        double sum = 0;
        for (int j = 0; j < N; ++j)
            sum += std::pow(x[j], i) * y[j];
        B[i][n+1] = sum;
    }
}

void PolynomialFitter::gaussianElimination() {
    int m = n + 1;
    // pivot and eliminate below
    for (int i = 0; i < m; ++i) {
        // pivot: find row with max abs in column i and swap
        int maxr = i;
        for (int k = i+1; k < m; ++k)
            if (std::fabs(B[k][i]) > std::fabs(B[maxr][i]))
                maxr = k;
        std::swap(B[i], B[maxr]);

        // eliminate rows below
        for (int k = i+1; k < m; ++k) {
            double t = B[k][i] / B[i][i];
            for (int j = i; j <= m; ++j)
                B[k][j] -= t * B[i][j];
        }
    }
}

void PolynomialFitter::backSubstitution() {
    int m = n + 1;
    for (int i = m - 1; i >= 0; --i) {
        a[i] = B[i][m];
        for (int j = i+1; j < m; ++j)
            a[i] -= B[i][j] * a[j];
        a[i] /= B[i][i];
    }
}
