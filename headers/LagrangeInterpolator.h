// LagrangeInterpolator.h
#ifndef LAGRANGE_INTERPOLATOR_H
#define LAGRANGE_INTERPOLATOR_H

#include <vector>

class LagrangeInterpolator {
public:
    LagrangeInterpolator(const std::vector<double>& xData, const std::vector<double>& yData);

    double interpolateY(double xValue) const;
    double interpolateX(double yValue) const;

private:
    std::vector<double> x;
    std::vector<double> y;
};

#endif
