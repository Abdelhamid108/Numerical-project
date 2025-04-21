#include "parser.h"
#include <cmath>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace std;

// Constructor
EquationParser::EquationParser() : allow_xy(false) {}

// Set the flag to allow or disallow both x and y in the same expression
void EquationParser::setAllowXY(bool allow) {
    allow_xy = allow;
}

// Check if the character is a valid operator
bool EquationParser::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Get the precedence of operators
int EquationParser::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Check if the string is a valid mathematical function
bool EquationParser::isFunction(const string& token) {
    for (const auto& func : math_functions) {
        if (token == func) return true;
    }
    return false;
}

// Check if the string is a constant (like pi or e)
bool EquationParser::isConstant(const string& token) {
    return constants.find(token) != constants.end();
}

// Validate the tokens to ensure the equation is valid
void EquationParser::validateTokens() {
    int paren_count = 0;
    bool has_x = false, has_y = false;

    for (size_t i = 0; i < tokens.size(); i++) {
        const string& token = tokens[i];

        if (token == "(") paren_count++;
        else if (token == ")") paren_count--;

        if (i > 0 && isOperator(token[0]) && isOperator(tokens[i - 1][0])) {
            throw runtime_error("Consecutive operators at position " + to_string(i));
        }

        if (isFunction(token)) {
            if (i == tokens.size() - 1 || tokens[i + 1] != "(") {
                throw runtime_error("Function '" + token + "' not followed by parentheses");
            }
        }

        if (token == "x") has_x = true;
        if (token == "y") has_y = true;
    }

    if (paren_count != 0) {
        throw runtime_error("Mismatched parentheses in equation");
    }

    if (!allow_xy && has_x && has_y) {
        throw runtime_error("Equation cannot contain both x and y at the same time.");
    }
}

// Parse the equation into tokens
void EquationParser::parseEquation(const string& equation) {
    tokens.clear();
    postfix.clear();

    for (size_t i = 0; i < equation.size(); ) {
        char c = equation[i];

        if (isspace(c)) {
            i++;
            continue;
        }

        if (isdigit(c) || c == '.') {
            string num;
            bool has_decimal = false, has_exponent = false;
            while (i < equation.size() && (isdigit(equation[i]) ||
                equation[i] == '.' || equation[i] == 'e' || equation[i] == 'E' ||
                ((equation[i] == '+' || equation[i] == '-') &&
                    (i > 0 && (equation[i - 1] == 'e' || equation[i - 1] == 'E'))))) {

                if (equation[i] == '.') {
                    if (has_decimal || has_exponent) throw runtime_error("Invalid number format");
                    has_decimal = true;
                }
                else if (equation[i] == 'e' || equation[i] == 'E') {
                    if (has_exponent) throw runtime_error("Invalid number format");
                    has_exponent = true;
                }
                num += equation[i++];
            }
            tokens.push_back(num);
            continue;
        }

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
            }
            else if (word == "y" || word == "Y") {
                tokens.push_back("y");
            }
            else {
                throw runtime_error("Unknown identifier: " + word);
            }
            continue;
        }

        if (isOperator(c) || c == '(' || c == ')') {
            if (c == '-' && (tokens.empty() || tokens.back() == "(" || isOperator(tokens.back()[0]))) {
                tokens.push_back("0");
                tokens.push_back("-");
                i++;
                continue;
            }
            else {
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

// Convert the equation to postfix notation
void EquationParser::convertToPostfix() {
    stack<string> op_stack;

    for (const auto& token : tokens) {
        if (token == "x" || token == "y" || isdigit(token[0]) ||
            (token[0] == '-' && token.size() > 1 && isdigit(token[1])) ||
            isConstant(token)) {
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

// Evaluate the postfix expression
double EquationParser::evaluate(double x_value) {
    bool has_x = false, has_y = false;
    for (const auto& token : postfix) {
        if (token == "x") has_x = true;
        if (token == "y") has_y = true;
    }

    if (!allow_xy && has_x && has_y) {
        throw runtime_error("This equation requires either x or y, not both.");
    } else if (has_x) {
        return evaluate(x_value, 0);
    } else if (has_y) {
        return evaluate(0, x_value);
    } else {
        return evaluate(0, 0);
    }
}

// Evaluate the equation with both x and y values
double EquationParser::evaluate(double x_value, double y_value) {
    stack<double> val_stack;

    for (const auto& token : postfix) {
        if (token == "x") {
            val_stack.push(x_value);
        }
        else if (token == "y") {
            val_stack.push(y_value);
        }
        else if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1 && isdigit(token[1]))) {
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
            else if (token == "asin") val_stack.push(asin(a));
            else if (token == "acos") val_stack.push(acos(a));
            else if (token == "atan") val_stack.push(atan(a));
            else if (token == "sinh") val_stack.push(sinh(a));
            else if (token == "cosh") val_stack.push(cosh(a));
            else if (token == "tanh") val_stack.push(tanh(a));
            else if (token == "sqrt") {
                if (a < 0) throw runtime_error("Square root of negative number");
                val_stack.push(sqrt(a));
            }
            else if (token == "exp") val_stack.push(exp(a));
            else if (token == "ln") {
                if (a <= 0) throw runtime_error("Logarithm of non-positive number");
                val_stack.push(log(a));
            }
            else if (token == "log") {
                if (a <= 0) throw runtime_error("Logarithm of non-positive number");
                val_stack.push(log10(a));
            }
        }
    }

    if (val_stack.size() != 1) throw runtime_error("Invalid expression");
    return val_stack.top();
}

// Print the postfix expression for debugging
void EquationParser::printPostfix() {
    cout << "Postfix notation: ";
    for (const auto& token : postfix) {
        cout << token << " ";
    }
    cout << endl;
}
