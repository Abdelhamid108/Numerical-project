#pragma once  

#ifndef _USE_MATH_DEFINES  
#define _USE_MATH_DEFINES  
#endif  

#include <cmath>  
#include <iostream>  
#include <stack>  
#include <vector>  
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

    bool allow_xy; // Flag to allow both x and y in the same expression

public:  
    EquationParser(); // Constructor

    // Set the flag to allow or disallow both x and y in the same expression
    void setAllowXY(bool allow); 

    void parseEquation(const std::string& equation);  
    void convertToPostfix();  
    double evaluate(double x_value);  
    double evaluate(double x_value, double y_value); // Evaluate for both x and y
    void printPostfix();  
};  

#endif // EQUATION_PARSER_H