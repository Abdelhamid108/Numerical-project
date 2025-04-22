// LagrangeInterpolator.cpp
#include "LagrangeInterpolator.h"

LagrangeInterpolator::LagrangeInterpolator(const std::vector<double>& xData, const std::vector<double>& yData)
    : x(xData), y(yData) {}

double LagrangeInterpolator::interpolateY(double xValue) const {
    int n = x.size();
    double result = 0.0;

    for (int i = 0; i < n; ++i) {
        double numerator = 1.0;
        double denominator = 1.0;

        for (int j = 0; j < n; ++j) {
            if (i != j) {
                numerator *= (xValue - x[j]);
                denominator *= (x[i] - x[j]);
            }
        }

        result += y[i] * (numerator / denominator);
    }

    return result;
}

double LagrangeInterpolator::interpolateX(double yValue) const {
    // Swap x and y, and interpolate normally
    int n = y.size();
    double result = 0.0;

    for (int i = 0; i < n; ++i) {
        double numerator = 1.0;
        double denominator = 1.0;

        for (int j = 0; j < n; ++j) {
            if (i != j) {
                numerator *= (yValue - y[j]);
                denominator *= (y[i] - y[j]);
            }
        }

        result += x[i] * (numerator / denominator);
    }

    return result;
}
