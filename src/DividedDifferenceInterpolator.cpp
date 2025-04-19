#include "DividedDifferenceInterpolator.h"

using namespace std;

double getValidatedDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) break;

        cout << "❌ Invalid input. Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

void divide::askP() {
    while (true) {
        cout << "Enter number of points (n > 1): ";
        cin >> n;
        if (!cin.fail() && n > 1 && n <= 20) break;

        cout << "❌ Invalid number. Please enter an integer > 1 and <= 20.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void divide::askX() {
    cout << endl;
    for (int i = 0; i < n; i++) {
        bool isDuplicate;
        do {
            isDuplicate = false;
            x[i] = getValidatedDouble("Enter X[" + to_string(i) + "]: ");

            for (int j = 0; j < i; j++) {
                if (abs(x[i] - x[j]) < 1e-9) {
                    cout << "❌ Duplicate value found! X values must be distinct.\n";
                    isDuplicate = true;
                    break;
                }
            }
        } while (isDuplicate);
    }
    cout << endl;
}

void divide::askF() {
    for (int j = 0; j < n; j++) {
        f[0][j] = getValidatedDouble("Enter F[" + to_string(j) + "]: ");
    }
    cout << endl;
}

void divide::askXX() {
    XX = getValidatedDouble("Enter value of X to evaluate f(X): ");

    double minX = x[0], maxX = x[0];
    for (int i = 1; i < n; i++) {
        if (x[i] < minX) minX = x[i];
        if (x[i] > maxX) maxX = x[i];
    }

    if (XX < minX || XX > maxX) {
        cout << "⚠️ Warning: X = " << XX << " is outside the interpolation range [" 
             << minX << ", " << maxX << "].\n";
        cout << "The result might be less accurate (this is extrapolation).\n\n";
    }
}

void divide::diffTable() {
    if (abs(XX - x[0]) < abs(XX - x[n - 1])) {
        // Forward difference table
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < n - i; j++) {
                f[i][j] = (f[i - 1][j + 1] - f[i - 1][j]) / (x[i + j] - x[j]);
            }
        }

        cout << endl << "Sn\tXi\tf(Xi)\t";
        for (int i = 0; i < n - 1; i++) cout << i + 1 << " diff\t";
        cout << endl;

        for (int i = 0; i < n; i++) {
            cout << i + 1 << "\t" << x[i] << "\t";
            for (int j = 0; j < n - i; j++) {
                cout << fixed << setprecision(4) << f[j][i] << "\t";
            }
            cout << endl;
        }
    } else {
        // Backward difference table
        for (int i = 1; i < n; i++) {
            for (int j = n - 1; j >= i; j--) {
                f[i][j] = (f[i - 1][j] - f[i - 1][j - 1]) / (x[j] - x[j - i]);
            }
        }

        cout << endl << "Sn\tXi\tf(Xi)\t";
        for (int i = 0; i < n - 1; i++) cout << i + 1 << " diff\t";
        cout << endl;

        for (int i = 0; i < n; i++) {
            cout << i + 1 << "\t" << x[i] << "\t";
            for (int j = 0; j <= i; j++) {
                cout << fixed << setprecision(4) << f[j][i] << "\t";
            }
            cout << endl;
        }
    }
}

void divide::calcP() {
    if (abs(XX - x[0]) < abs(XX - x[n - 1])) {
        // Forward interpolation
        P1 = 0;
        for (int i = 0; i < n; i++) {
            double k = 1;
            for (int j = 0; j < i; j++) {
                k *= (XX - x[j]);
            }
            P1 += k * f[i][0];
        }
    } else {
        // Backward interpolation
        P1 = 0;
        for (int i = 0; i < n; i++) {
            double k = 1;
            for (int j = 0; j < i; j++) {
                k *= (XX - x[n - 1 - j]);
            }
            P1 += k * f[i][n - 1];
        }
    }

    cout << endl << "The value of P" << n - 1 << "(" << XX << "): " 
         << fixed << setprecision(6) << P1 << endl << endl;
}

divide::divide()
{
    askP();
    askX();
    askF();
    askXX();
    diffTable();
    calcP();
}
