#include "integration.h"

using namespace std;

double NumericalIntegrator::getBoundInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        try {
            size_t pos;
            double value = stod(input, &pos);
            if (pos == input.length()) return value;
            
            EquationParser bound_parser;
            bound_parser.parseEquation(input);
            return bound_parser.evaluate(0);
        } catch (const exception& e) {
            cout << "Invalid input (" << e.what() << "). Please try again.\n";
        }
    }
}

void NumericalIntegrator::getValidInput(const string& prompt, int& value, int min_val) {
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min_val) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer >= " << min_val << ".\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}

void NumericalIntegrator::generatePoints() {
    h = (b - a) / (n - 1);
    x.resize(n);
    fx.resize(n);
    
    for (int i = 0; i < n; i++) {
        x[i] = a + i * h;
        try {
            fx[i] = parser.evaluate(x[i]);
        } catch (const exception& e) {
            cout << "Error evaluating at x = " << x[i] << ": " << e.what() << endl;
            throw;
        }
    }
}

void NumericalIntegrator::displayTable() {
    cout << "\nGenerated Points Table:\n";
    cout << "Index\tx\t\tf(x)\n";
    cout << fixed << setprecision(6);
    
    int show_points = min(n, 10);
    for (int i = 0; i < show_points/2; i++) {
        cout << i << "\t" << x[i] << "\t" << fx[i] << endl;
    }
    if (n > show_points) {
        cout << "...\t...\t\t...\n";
        for (int i = n - show_points/2; i < n; i++) {
            cout << i << "\t" << x[i] << "\t" << fx[i] << endl;
        }
    }
}

double NumericalIntegrator::trapezoidalRule() {
    double sum = fx[0] + fx[n-1];
    for (int i = 1; i < n-1; i++) sum += 2 * fx[i];
    return (h / 2.0) * sum;
}

double NumericalIntegrator::simpsons13Rule() {
    if ((n - 1) % 2 != 0) throw runtime_error("Simpson's 1/3 needs even intervals");
    
    double sum = fx[0] + fx[n-1];
    for (int i = 1; i < n-1; i++) sum += (i % 2 == 0) ? 2 * fx[i] : 4 * fx[i];
    return (h / 3.0) * sum;
}

double NumericalIntegrator::simpsons38Rule() {
    if ((n - 1) % 3 != 0) throw runtime_error("Simpson's 3/8 needs intervals divisible by 3");
    
    double sum = fx[0] + fx[n-1];
    for (int i = 1; i < n-1; i++) sum += (i % 3 == 0) ? 2 * fx[i] : 3 * fx[i];
    return (3.0 * h / 8.0) * sum;
}

void NumericalIntegrator::run() {
    cout << "==== Numerical Integration Calculator ====\n";
    
    // Get equation
    while (true) {
        try {
            cout << "\nEnter equation (e.g., exp((-x)^2)): ";
            string equation;
            getline(cin, equation);
            parser.parseEquation(equation);
            break;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\nPlease try again.\n";
        }
    }
    
    // Get bounds
    a = getBoundInput("Enter lower bound (a): ");
    b = getBoundInput("Enter upper bound (b): ");
    while (b <= a) {
        cout << "Upper bound must be greater than lower bound.\n";
        b = getBoundInput("Enter upper bound (b): ");
    }
    
    // Get number of points
    getValidInput("Enter number of points (>=2): ", n, 2);
    
    // Generate points
    try {
        generatePoints();
    } catch (...) {
        return;
    }
    
    // Display table
    displayTable();
    
    // Integration method selection
    int choice;
    do {
        cout << "\nChoose integration method:\n";
        cout << "1. Trapezoidal Rule\n";
        cout << "2. Simpson's 1/3 Rule\n";
        cout << "3. Simpson's 3/8 Rule\n";
        cout << "4. All Methods\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        
        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter 1-5: ";
        }
        cin.ignore();
        
        try {
            switch (choice) {
                case 1:
                    cout << "\nTrapezoidal Rule Result: " << trapezoidalRule() << endl;
                    break;
                case 2:
                    cout << "\nSimpson's 1/3 Rule Result: " << simpsons13Rule() << endl;
                    break;
                case 3:
                    cout << "\nSimpson's 3/8 Rule Result: " << simpsons38Rule() << endl;
                    break;
                case 4:
                    cout << "\nAll Integration Methods:\n";
                    cout << "Trapezoidal Rule: " << trapezoidalRule() << endl;
                    cout << "Simpson's 1/3 Rule: " << simpsons13Rule() << endl;
                    cout << "Simpson's 3/8 Rule: " << simpsons38Rule() << endl;
                    break;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 5);
}