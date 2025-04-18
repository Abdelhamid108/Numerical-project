#include "parser.h"
#include <stack>
#include <cctype>
#include <cmath>
#include <map>

using namespace std;

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
    return token == "sin" || token == "cos" || token == "exp" || token == "log" || token == "sqrt";
}

vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    for (size_t i = 0; i < expr.length(); ) {
        if (isspace(expr[i])) {
            ++i;
        } else if (isdigit(expr[i]) || expr[i] == '.') {
            string num;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
                num += expr[i++];
            tokens.push_back(num);
        } else if (isalpha(expr[i])) {
            string func;
            while (i < expr.length() && isalpha(expr[i]))
                func += expr[i++];
            tokens.push_back(func);
        } else if (isOperator(expr[i]) || expr[i] == '(' || expr[i] == ')') {
            tokens.push_back(string(1, expr[i++]));
        } else {
            ++i; 
        }
    }
    return tokens;
}

vector<string> toPostfix(const vector<string>& tokens) {
    vector<string> output;
    stack<string> ops;

    for (const auto& token : tokens) {
        if (token == "x" || isdigit(token[0])) {
            output.push_back(token);
        } else if (isFunction(token)) {
            ops.push(token);
        } else if (token == ",") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
        } else if (isOperator(token[0])) {
            while (!ops.empty() && (
                (isOperator(ops.top()[0]) && precedence(ops.top()[0]) >= precedence(token[0])) ||
                isFunction(ops.top())
            )) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty()) ops.pop(); // remove '('
            if (!ops.empty() && isFunction(ops.top())) {
                output.push_back(ops.top());
                ops.pop();
            }
        }
    }

    while (!ops.empty()) {
        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}

double evaluatePostfix(const vector<string>& postfix, double x) {
    stack<double> st;

    for (const auto& token : postfix) {
        if (token == "x") {
            st.push(x);
        } else if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-')) {
            st.push(stod(token));
        } else if (isOperator(token[0]) && token.size() == 1) {
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            switch (token[0]) {
                case '+': st.push(a + b); break;
                case '-': st.push(a - b); break;
                case '*': st.push(a * b); break;
                case '/': st.push(a / b); break;
                case '^': st.push(pow(a, b)); break;
            }
        } else if (isFunction(token)) {
            double a = st.top(); st.pop();
            if (token == "sin") st.push(sin(a));
            else if (token == "cos") st.push(cos(a));
            else if (token == "exp") st.push(exp(a));
            else if (token == "log") st.push(log(a));
            else if (token == "sqrt") st.push(sqrt(a));
        }
    }

    return st.top();
}

double f(const string& expr, double x) {
    auto tokens = tokenize(expr);
    auto postfix = toPostfix(tokens);
    return evaluatePostfix(postfix, x);
}
