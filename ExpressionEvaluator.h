#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H

#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include "LinkedList.h" // To find other tasks' priorities
#include "Task.h"
#include "Stack.h"

class ExpressionEvaluator 
{
public:
    static int evaluate(const std::string& expression, LinkedList<Task>* allTasks);

private:
    static int precedence(char op);
    static double applyOp(double a, double b, char op);
    static std::string infixToPostfix(const std::string& expression, LinkedList<Task>* allTasks);
};

int ExpressionEvaluator::precedence(char op) 
{
    if (op == '+' || op == '-') 
        return 1;
    if (op == '*' || op == '/') 
        return 2;
    return 0;
}

double ExpressionEvaluator::applyOp(double a, double b, char op) 
{
    switch (op) 
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

std::string ExpressionEvaluator::infixToPostfix(const std::string& expression, LinkedList<Task>* allTasks) 
{
    Stack<char> ops;
    std::stringstream postfix;
    std::string processedExpr = expression;
    
    // Find and replace "priority of TXXX" with the actual priority value
    size_t pos = 0;
    while ((pos = processedExpr.find("priority of T", pos)) != std::string::npos) 
    {
        size_t startId = pos + 12;
        size_t endId = processedExpr.find_first_not_of("0123456789", startId);
        std::string taskId = processedExpr.substr(startId, endId - startId);
        Task* dependencyTask = allTasks->findTask("T" + taskId);
        if (dependencyTask) 
        {
            processedExpr.replace(pos, (endId != std::string::npos ? endId : processedExpr.length()) - pos, std::to_string(dependencyTask->calculatedPriority));
        } 
        else 
        {
             throw std::runtime_error("Dependency task for priority calculation not found: T" + taskId);
        }
    }

    std::stringstream ss(processedExpr);
    char token;
    double num;
    
    // A simplified parser assuming space-separated tokens
    std::string current_token;
    std::stringstream token_stream(processedExpr);

    while(token_stream >> current_token) 
    {
        if(isdigit(current_token[0]) || (current_token.length() > 1 && current_token[0] == '.')) 
        {
             postfix << current_token << " ";
        } 
        else if(current_token.length() == 1 && std::string("+-*/").find(current_token[0]) != std::string::npos) 
        {
             char op = current_token[0];
             while(!ops.isEmpty() && precedence(ops.top()) >= precedence(op)) 
             {
                 postfix << ops.pop() << " ";
             }
             ops.push(op);
        }
    }

    while (!ops.isEmpty()) 
    {
        postfix << ops.pop() << " ";
    }
    
    return postfix.str();
}


int ExpressionEvaluator::evaluate(const std::string& expression, LinkedList<Task>* allTasks) {
    // If expression is just a number
    try 
    {
        size_t processed = 0;
        int direct_priority = std::stoi(expression, &processed);
        if (processed == expression.length()) 
        {
            return direct_priority;
        }
    } catch (...) 
    {}
    
    std::string postfix = infixToPostfix(expression, allTasks);
    Stack<double> values;
    std::stringstream ss(postfix);
    std::string token;

    while (ss >> token) 
    {
        if (isdigit(token[0])) 
        {
            values.push(std::stod(token));
        } else 
        {
            double val2 = values.pop();
            double val1 = values.pop();
            values.push(applyOp(val1, val2, token[0]));
        }
    }
    return static_cast<int>(round(values.pop()));
}

#endif