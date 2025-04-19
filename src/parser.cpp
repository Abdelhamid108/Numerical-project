#include "parser.h"

using namespace std;

bool EquationParser::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int EquationParser::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool EquationParser::isFunction(const string& token) {
    for (const auto& func : math_functions) {
        if (token == func) return true;
    }
    return false;
}

bool EquationParser::isConstant(const string& token) {
    return constants.find(token) != constants.end();
}

void EquationParser::validateTokens() {
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

void EquationParser::parseEquation(const string& equation) {
    tokens.clear();
    postfix.clear();
    
    for (size_t i = 0; i < equation.size(); ) {
        char c = equation[i];
        
        if (isspace(c)) {
            i++;
            continue;
        }
        
        // Handle numbers (including scientific notation)
        if (isdigit(c) || c == '.') {
            string num;
            bool has_decimal = false, has_exponent = false;
            while (i < equation.size() && (isdigit(equation[i]) || 
                   equation[i] == '.' || equation[i] == 'e' || equation[i] == 'E' ||
                   ((equation[i] == '+' || equation[i] == '-') && 
                    (i > 0 && (equation[i-1] == 'e' || equation[i-1] == 'E'))))) {
                
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
                    } else if (isdigit(equation[i+1]) || equation[i+1] == '.') {
                        // Let the number parsing handle it next iteration
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

void EquationParser::convertToPostfix() {
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

double EquationParser::evaluate(double x_value) {
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

void EquationParser::printPostfix() {
    cout << "Postfix notation: ";
    for (const auto& token : postfix) {
        cout << token << " ";
    }
    cout << endl;
}
