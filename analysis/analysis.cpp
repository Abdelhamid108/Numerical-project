#include <iostream>
#include <conio.h>
#include <cmath>
#include <limits>
#include<string>
#include<iomanip>
#include<sstream>
using namespace std;

double getValidatedDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) break;

        cout << "❌ Invalid input. Please enter a valid number.\n";
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore rest of line
    }
    return value;
}

class divide
{
public:
    void askP();      // asks for number of points
    void askX();      // asks for x values
    void askF();      // asks for y values
    void askXX();     // asks for x at which you want to calulate y
    void diffTable(); // calculate and display table
    void calcP();     // calculate and display the desired y
private:
    double XX, x[20], f[20][20], P1;
    int n;

};

void divide::askP()
{
    while (true) {
        cout << "Enter number of points (n > 1): ";
        cin >> n;
        if (!cin.fail() && n > 1 && n <= 20) break;

        cout << "❌ Invalid number. Please enter an integer > 1 and <= 20.\n";
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore rest of line
    }
}

void divide::askX()
{
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        bool isDuplicate;
        do {
            isDuplicate = false;
            x[i] = getValidatedDouble("Enter X[" + to_string(i) + "]: ");

            // Check if value already exists in previous x values
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

void divide::askF()
{

    for (int j = 0; j < n; j++)
    {
        f[0][j] = getValidatedDouble("Enter F[" + to_string(j) + "]: ");
    }
    cout << endl;
}
void divide::askXX()
{

    XX = getValidatedDouble("Enter value of X to evaluate f(X): ");

    // Check if XX is out of interpolation range
    double minX = x[0], maxX = x[0];
    for (int i = 1; i < n; i++) {
        if (x[i] < minX) minX = x[i];
        if (x[i] > maxX) maxX = x[i];
    }

    if (XX < minX || XX > maxX) {
        cout << "⚠️ Warning: X = " << XX << " is outside the interpolation range [" << minX << ", " << maxX << "].\n";
        cout << "The result might be less accurate (this is extrapolation).\n\n";
    }
}

void divide::diffTable()
{
    if (abs(XX - x[0]) < abs(XX - x[n - 1]))    // check to decide forward or backward disblay table
    {
        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < n - i; j++)
            {
                f[i][j] = (f[i - 1][j + 1] - f[i - 1][j]) / (x[i + j] - x[j]);  // calculate deltas such as [delta = (y1 - y0)/(x1 - x0)] and so on
            }
        }
        cout << endl;
        cout << "Sn\tXi\tf(Xi)\t";
        for (int i = 0; i < n - 1; i++)
        {
            cout << i + 1 << " diff\t";
        }
        cout << endl;
        for (int i = 0; i < n; i++)
        {
            cout << i + 1 << "\t" << x[i] << "\t";
            for (int j = 0; j < n - i; j++)
            {
                cout << fixed << setprecision(4) << f[j][i] << "\t";
            }
            cout << endl;
        }
    }
    else
    {
        for (int i = 1; i < n; i++)
        {
            for (int j = n - 1; j >= i; j--)
            {
                f[i][j] = (f[i - 1][j] - f[i - 1][j - 1]) / (x[j] - x[j - i]);
            }
        }
        cout << endl;
        cout << "Sn\tXi\tf(Xi)\t";
        for (int i = 0; i < n - 1; i++)
        {
            cout << i + 1 << " diff\t";
        }
        cout << endl;
        for (int i = 0; i < n; i++)
        {
            cout << i + 1 << "\t" << x[i] << "\t";
            for (int j = 0; j <= i; j++)
            {
                cout << fixed << setprecision(4) << f[j][i] << "\t";
            }
            cout << endl;
        }
    }
}

void divide::calcP()
{
    if (abs(XX - x[0]) < abs(XX - x[n - 1]))
    {
        P1 = 0;
        for (int i = 0; i < n; i++)
        {
            double k = 1;
            for (int j = 0; j < i; j++)
            {
                k *= (XX - x[j]);
            }
            P1 += k * f[i][0];  // divided forward formula y = y0 + delta * (x-x0) + delta * (x-x0) * (x-x1) and so on

        }
        cout << endl << "The value of P" << n - 1 << "(" << XX << "): " << P1 << endl << endl;
    }
    else
    {


        P1 = 0;
        for (int i = 0; i < n; i++)
        {
            double k = 1;
            for (int j = 0; j < i; j++)
            {
                k *= (XX - x[n - 1 - j]);
            }
            P1 += k * f[i][n - 1];  // divide backward formula y = y3 + delta * (x-x3) + delta * (x-x3) * (x-x2) and so on

        }
        cout << endl << "The value of P" << n - 1 << "(" << XX << "): " << fixed << setprecision(6) << P1 << endl << endl; //✅ formatted result
    }
}


int main()
{

    divide d1;
    d1.askP();
    d1.askX();
    d1.askF();
    d1.askXX();
    d1.diffTable();
    d1.calcP();

}