#ifndef EQUATION_PARSER_H
#define EQUATION_PARSER_H

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

class EquationParser {
private:
    std::vector<std::string> tokens;
    std::vector<std::string> postfix;
    
    const std::vector<std::string> math_functions = {
        "sin", "cos", "tan", "asin", "acos", "atan",
        "sinh", "cosh", "tanh", "sqrt", "exp", "log", "log10"
    };
    
    const std::map<std::string, double> constants = {
        {"pi", M_PI}, {"e", M_E}
    };

    bool isOperator(char c);
    int precedence(char op);
    bool isFunction(const std::string& token);
    bool isConstant(const std::string& token);
    void validateTokens();

public:
    void parseEquation(const std::string& equation);
    void convertToPostfix();
    double evaluate(double x_value);
    void printPostfix();
};

#endif // EQUATION_PARSER_H