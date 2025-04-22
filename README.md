# EquationParser Documentation

## Overview

The `EquationParser` class is designed to parse and evaluate mathematical equations. It supports equations containing variables like `x` and `y`, constants such as `pi` and `e`, and various mathematical functions such as `sin`, `cos`, `tan`, etc. This class can process both infix and postfix notation, allowing for easy evaluation of complex expressions.

## Features

- Supports equations with both `x` and `y` together or separately.
- Handles basic arithmetic operations: addition, subtraction, multiplication, division, and exponentiation.
- Supports mathematical functions such as `sin`, `cos`, `tan`, `sqrt`, `exp`, `ln`, `log`, and more.
- Handles mathematical constants like `pi` and `e`.
- Converts equations from standard infix notation to postfix notation for easier evaluation.
- Supports equations containing numbers, operators, and mathematical functions.

## Public Methods

### 1. `EquationParser()`
   **Description**: Constructor for creating an instance of `EquationParser`.  
   **Usage**: 
   ```cpp
   EquationParser parser;
2. void setAllowXY(bool allow)
Description: This method allows you to specify whether both x and y can be used together in the equation.
Parameters:

allow: A boolean value (true to allow both x and y in the same equation, false to disallow it).
Usage:

cpp
Copy
Edit
parser.setAllowXY(true);  // Allow x and y to be used together
3. void parseEquation(const std::string& equation)
Description: Parses the input equation and converts it into tokens. This method prepares the equation for evaluation.
Parameters:

equation: A string representing the mathematical equation to be parsed.
Usage:

cpp
Copy
Edit
parser.parseEquation("sin(x) + cos(y)");  // Parse an equation
4. void convertToPostfix()
Description: Converts the equation into postfix notation (Reverse Polish Notation) for easier evaluation.
Usage: This method is automatically called within parseEquation to convert the parsed equation to postfix.

5. double evaluate(double x_value)
Description: Evaluates the parsed equation for the given value of x. If y is used in the equation, it assumes y = 0 by default.
Parameters:

x_value: The value of x to be used in the equation.
Usage:

cpp
Copy
Edit
double result = parser.evaluate(5.0);  // Evaluate for x = 5
6. double evaluate(double x_value, double y_value)
Description: Evaluates the parsed equation for both x and y values.
Parameters:

x_value: The value of x to be used in the equation.

y_value: The value of y to be used in the equation.
Usage:

cpp
Copy
Edit
double result = parser.evaluate(5.0, 3.0);  // Evaluate for x = 5 and y = 3
7. void printPostfix()
Description: Prints the equation in its postfix notation.
Usage:

cpp
Copy
Edit
parser.printPostfix();  // Print the postfix notation of the equation
Private Methods
1. bool isOperator(char c)
Description: Checks if a character is a valid operator (+, -, *, /, ^).
Usage:

cpp
Copy
Edit
bool result = parser.isOperator('+');  // Check if '+' is an operator
2. int precedence(char op)
Description: Returns the precedence of the given operator. Higher numbers indicate higher precedence.
Usage:

cpp
Copy
Edit
int precedence = parser.precedence('*');  // Get the precedence of multiplication
3. bool isFunction(const std::string& token)
Description: Checks if a given token is a mathematical function (e.g., sin, cos, sqrt, etc.).
Usage:

cpp
Copy
Edit
bool result = parser.isFunction("sin");  // Check if 'sin' is a function
4. bool isConstant(const std::string& token)
Description: Checks if a given token is a recognized constant (e.g., pi, e).
Usage:

cpp
Copy
Edit
bool result = parser.isConstant("pi");  // Check if 'pi' is a constant
5. void validateTokens()
Description: Validates the tokens generated during parsing to ensure they form a valid expression.

6. void parseEquation(const std::string& equation)
Description: Splits the equation into tokens for further processing.

7. void convertToPostfix()
Description: Converts the infix equation to postfix notation.

How to Use
1. Creating an instance:
Start by creating an object of EquationParser.

cpp
Copy
Edit
EquationParser parser;
2. Setting Allowable Variables:
If you want to allow both x and y in the same equation, set allowXY to true.

cpp
Copy
Edit
parser.setAllowXY(true);
3. Parsing the Equation:
Use the parseEquation method to parse your equation.

cpp
Copy
Edit
parser.parseEquation("sin(x) + cos(y)");
4. Evaluating the Equation:
After parsing the equation, you can evaluate it by passing the required values for x and/or y.

cpp
Copy
Edit
double result = parser.evaluate(5.0, 3.0);  // Evaluate for x=5 and y=3
5. Printing the Postfix Expression:
To see the equation in postfix notation, call printPostfix.

cpp
Copy
Edit
parser.printPostfix();
Example
cpp
Copy
Edit
#include "EquationParser.h"
#include <iostream>

int main() {
    // Create the parser
    EquationParser parser;

    // Allow both x and y in the equation
    parser.setAllowXY(true);

    // Parse the equation
    parser.parseEquation("sin(x) + cos(y)");

    // Evaluate the equation for specific values of x and y
    double result = parser.evaluate(5.0, 3.0);  // x = 5, y = 3

    // Print the result
    std::cout << "Result: " << result << std::endl;

    return 0;
}
Conclusion
This EquationParser class is a flexible tool for parsing and evaluating mathematical expressions involving variables (x and y), constants, and functions. You can easily adapt it to handle more complex expressions, ensuring efficient and accurate evaluation of mathematical models.
