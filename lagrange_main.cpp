// main.cpp
#include <iostream>
#include <vector>
#include <iomanip>
#include "LagrangeInterpolator.h"

using namespace std;

int main() {
    int n, choice;
    double inputValue, outputValue;

    cout << "------ Lagrange Interpolation Program (C++) ------\n\n";
    cout << "1. Calculate y for a given x\n";
    cout << "2. Calculate x for a given y (inverse interpolation)\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    cout << "\nEnter the number of data points: ";
    cin >> n;

    vector<double> x(n), y(n);

    cout << "\nEnter the values of X:\n";
    for (int i = 0; i < n; ++i) {
        cout << "  x[" << i << "] = ";
        cin >> x[i];
    }

    cout << "\nEnter the values of Y:\n";
    for (int i = 0; i < n; ++i) {
        cout << "  y[" << i << "] = ";
        cin >> y[i];
    }

    LagrangeInterpolator interpolator(x, y);

    cout << fixed << setprecision(6);

    if (choice == 1) {
        cout << "\nEnter the value of x to interpolate y: ";
        cin >> inputValue;
        outputValue = interpolator.interpolateY(inputValue);
        cout << "\nInterpolated value at x = " << inputValue << " is y(x) = " << outputValue << endl;
    }
    else if (choice == 2) {
        cout << "\nEnter the value of y to interpolate x: ";
        cin >> inputValue;
        outputValue = interpolator.interpolateX(inputValue);
        cout << "\nInterpolated value at y = " << inputValue << " is x(y) = " << outputValue << endl;
    }
    else {
        cout << "Invalid choice.\n";
    }

    return 0;
}
