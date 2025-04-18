# project
# Equation Parser Documentation

## Overview
The `EquationParser` class provides a robust solution for parsing and evaluating mathematical expressions. It supports variables, constants, functions, and standard arithmetic operations with proper operator precedence and parentheses handling.

## Features
- Parses mathematical expressions with variables (`x`)
- Supports standard arithmetic operations (`+`, `-`, `*`, `/`, `^`)
- Handles 13 mathematical functions (trigonometric, hyperbolic, exponential, logarithmic)
- Includes mathematical constants (`pi`, `e`)
- Validates expression syntax
- Converts to postfix notation (RPN)
- Evaluates expressions for given variable values

## Class Members

### Private Members
- `tokens`: Stores tokenized input expression
- `postfix`: Stores expression in postfix notation
- `math_functions`: Supported mathematical functions
- `constants`: Supported mathematical constants

### Private Methods
- `isOperator(char c)`: Checks if character is an operator
- `precedence(char op)`: Returns operator precedence level
- `isFunction(const string& token)`: Checks if token is a supported function
- `isConstant(const string& token)`: Checks if token is a supported constant
- `validateTokens()`: Validates token sequence

### Public Methods
- `parseEquation(const string& equation)`: Parses input equation
- `convertToPostfix()`: Converts tokens to postfix notation
- `evaluate(double x_value)`: Evaluates expression for given x value
- `printPostfix()`: Prints postfix notation (for debugging)

## Usage Example

```cpp
#include "EquationParser.h"
#include <iostream>

int main() {
    EquationParser parser;
    
    try {
        // Parse and evaluate an expression
        parser.parseEquation("sin(x) + 2*cos(pi/2)");
        parser.printPostfix();
        
        double result = parser.evaluate(1.0);  // Evaluate at x=1.0
        std::cout << "Result: " << result << std::endl;
        
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## Supported Functions
| Function | Description |
|----------|-------------|
| `sin`    | Sine |
| `cos`    | Cosine |
| `tan`    | Tangent |
| `asin`   | Arcsine |
| `acos`   | Arccosine |
| `atan`   | Arctangent |
| `sinh`   | Hyperbolic sine |
| `cosh`   | Hyperbolic cosine |
| `tanh`   | Hyperbolic tangent |
| `sqrt`   | Square root |
| `exp`    | Exponential |
| `log`    | Natural logarithm |
| `log10`  | Base-10 logarithm |

## Supported Constants
| Constant | Value |
|----------|-------|
| `pi`     | π (3.14159...) |
| `e`      | Euler's number (2.71828...) |

## Error Handling
The class throws `runtime_error` exceptions for:
- Invalid characters in input
- Mismatched parentheses
- Unknown identifiers
- Invalid number formats
- Mathematical errors (division by zero, log of non-positive, etc.)
- Syntax errors (consecutive operators, missing parentheses)

## Implementation Notes
- Uses shunting-yard algorithm for infix to postfix conversion
- Handles unary minus operations
- Supports scientific notation in numbers
- Maintains strict operator precedence

## Limitations
- Single variable (`x`) support only
- No implicit multiplication
- No multi-character variable names
- No user-defined functions or constants
