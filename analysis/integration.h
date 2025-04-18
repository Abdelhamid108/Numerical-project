#include <iostream>
#include <stack>
#include <vector>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <map>
#include <limits>

using namespace std;

class EquationParser {
private:
    vector<string> tokens;
    vector<string> postfix;
    
    const vector<string> math_functions = {"sin", "cos", "tan", "sqrt", "exp", "log"};
    const map<string, double> constants = {{"pi", M_PI}, {"e", M_E}};

    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '^') return 3;
        return 0;
    }

    bool isFunction(const string& token) {
        for (const auto& func : math_functions) {
            if (token == func) return true;
        }
        return false;
    }

    bool isConstant(const string& token) {
        return constants.find(token) != constants.end();
    }

    void validateTokens() {
        int paren_count = 0;
        for (size_t i = 0; i < tokens.size(); i++) {
            const string& token = tokens[i];
            
            if (token == "(") paren_count++;
            else if (token == ")") paren_count--;
            
            if (i > 0 && isOperator(token[0]) && isOperator(tokens[i-1][0])) {
                throw runtime_error("Consecutive operators at position " + to_string(i));
            }
            
            if (isFunction(token)) {
                if (i == tokens.size() - 1 || tokens[i+1] != "(") {
                    throw runtime_error("Function '" + token + "' not followed by parentheses");
                }
            }
        }
        
        if (paren_count != 0) {
            throw runtime_error("Mismatched parentheses in equation");
        }
    }

public:
    void parseEquation(const string& equation) {
        tokens.clear();
        postfix.clear();
        
        for (size_t i = 0; i < equation.size(); ) {
            char c = equation[i];
            
            if (isspace(c)) {
                i++;
                continue;
            }
            
            // Handle numbers
            if (isdigit(c) || c == '.') {
                string num;
                bool has_decimal = false;
                while (i < equation.size() && (isdigit(equation[i]) || equation[i] == '.' || 
                       equation[i] == 'e' || equation[i] == 'E')) {
                    if (equation[i] == '.') {
                        if (has_decimal) throw runtime_error("Invalid number format");
                        has_decimal = true;
                    }
                    num += equation[i++];
                }
                tokens.push_back(num);
                continue;
            }
            
            // Handle variables, functions, and constants
            if (isalpha(c)) {
                string word;
                while (i < equation.size() && isalpha(equation[i])) {
                    word += equation[i++];
                }
                
                if (isFunction(word)) {
                    tokens.push_back(word);
                } 
                else if (isConstant(word)) {
                    tokens.push_back(word);
                }
                else if (word == "x" || word == "X") {
                    tokens.push_back("x");
                } else {
                    throw runtime_error("Unknown identifier: " + word);
                }
                continue;
            }
            
            // Handle operators and parentheses
            if (isOperator(c) || c == '(' || c == ')') {
                // Handle unary minus
                if (c == '-' && (tokens.empty() || tokens.back() == "(" || 
                    isOperator(tokens.back()[0]))) {
                    tokens.push_back("(");
                    tokens.push_back("0");
                    tokens.push_back("-");
                    // Get the operand
                    if (i+1 < equation.size()) {
                        if (equation[i+1] == '(') {
                            tokens.push_back("(");
                            i++;
                        } else if (equation[i+1] == 'x') {
                            tokens.push_back("x");
                            i++;
                        }
                    }
                    tokens.push_back(")");
                    i++;
                } else {
                    tokens.push_back(string(1, c));
                    i++;
                }
                continue;
            }
            
            throw runtime_error(string("Invalid character: '") + c + "'");
        }
        
        validateTokens();
        convertToPostfix();
    }

    void convertToPostfix() {
        stack<string> op_stack;
        
        for (const auto& token : tokens) {
            if (token == "x" || isdigit(token[0]) || (token[0] == '-' && token.size() > 1) || isConstant(token)) {
                postfix.push_back(token);
            }
            else if (isFunction(token)) {
                op_stack.push(token);
            }
            else if (token == "(") {
                op_stack.push(token);
            }
            else if (token == ")") {
                while (!op_stack.empty() && op_stack.top() != "(") {
                    postfix.push_back(op_stack.top());
                    op_stack.pop();
                }
                if (op_stack.empty()) throw runtime_error("Mismatched parentheses");
                op_stack.pop();
                
                if (!op_stack.empty() && isFunction(op_stack.top())) {
                    postfix.push_back(op_stack.top());
                    op_stack.pop();
                }
            }
            else if (isOperator(token[0])) {
                while (!op_stack.empty() && op_stack.top() != "(" &&
                      ((isOperator(op_stack.top()[0]) && precedence(op_stack.top()[0]) >= precedence(token[0])) ||
                       isFunction(op_stack.top()))) {
                    postfix.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.push(token);
            }
        }
        
        while (!op_stack.empty()) {
            if (op_stack.top() == "(") throw runtime_error("Mismatched parentheses");
            postfix.push_back(op_stack.top());
            op_stack.pop();
        }
    }

    double evaluate(double x_value) {
        stack<double> val_stack;
        
        for (const auto& token : postfix) {
            if (token == "x") {
                val_stack.push(x_value);
            }
            else if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
                val_stack.push(stod(token));
            }
            else if (isConstant(token)) {
                val_stack.push(constants.at(token));
            }
            else if (isOperator(token[0])) {
                if (val_stack.size() < 2) throw runtime_error("Not enough operands");
                
                double b = val_stack.top(); val_stack.pop();
                double a = val_stack.top(); val_stack.pop();
                
                switch (token[0]) {
                    case '+': val_stack.push(a + b); break;
                    case '-': val_stack.push(a - b); break;
                    case '*': val_stack.push(a * b); break;
                    case '/': 
                        if (b == 0) throw runtime_error("Division by zero");
                        val_stack.push(a / b); 
                        break;
                    case '^': val_stack.push(pow(a, b)); break;
                }
            }
            else if (isFunction(token)) {
                if (val_stack.empty()) throw runtime_error("Not enough operands");
                
                double a = val_stack.top(); val_stack.pop();
                
                if (token == "sin") val_stack.push(sin(a));
                else if (token == "cos") val_stack.push(cos(a));
                else if (token == "tan") val_stack.push(tan(a));
                else if (token == "sqrt") {
                    if (a < 0) throw runtime_error("Square root of negative number");
                    val_stack.push(sqrt(a));
                }
                else if (token == "exp") val_stack.push(exp(a));
                else if (token == "log") {
                    if (a <= 0) throw runtime_error("Logarithm of non-positive number");
                    val_stack.push(log(a));
                }
            }
        }
        
        if (val_stack.size() != 1) throw runtime_error("Invalid expression");
        return val_stack.top();
    }

    void printPostfix() {
        cout << "Postfix notation: ";
        for (const auto& token : postfix) {
            cout << token << " ";
        }
        cout << endl;
    }
};

class NumericalIntegrator {
private:
    EquationParser parser;
    double a, b;
    int n;
    vector<double> x, fx;
    double h;

    double getBoundInput(const string& prompt) {
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

    void getValidInput(const string& prompt, int& value, int min_val) {
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

    void generatePoints() {
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

    void displayTable() {
        cout << "\nGenerated Points Table:\n";
        cout << "Index\tx\t\tf(x)\n";
        cout << fixed << setprecision(6);
        
        // Show first 5 and last 5 points if there are many
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

    double trapezoidalRule() {
        double sum = fx[0] + fx[n-1];
        for (int i = 1; i < n-1; i++) sum += 2 * fx[i];
        return (h / 2.0) * sum;
    }

    double simpsons13Rule() {
        if ((n - 1) % 2 != 0) throw runtime_error("Simpson's 1/3 needs even intervals");
        
        double sum = fx[0] + fx[n-1];
        for (int i = 1; i < n-1; i++) sum += (i % 2 == 0) ? 2 * fx[i] : 4 * fx[i];
        return (h / 3.0) * sum;
    }

    double simpsons38Rule() {
        if ((n - 1) % 3 != 0) throw runtime_error("Simpson's 3/8 needs intervals divisible by 3");
        
        double sum = fx[0] + fx[n-1];
        for (int i = 1; i < n-1; i++) sum += (i % 3 == 0) ? 2 * fx[i] : 3 * fx[i];
        return (3.0 * h / 8.0) * sum;
    }

public:
    void run() {
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
};

/*int main() {
    NumericalIntegrator integrator;
    integrator.run();
    return 0;
}*/