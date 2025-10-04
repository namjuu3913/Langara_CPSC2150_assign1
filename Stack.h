#pragma once
#include <string>
#include <vector>
#include <stack>
#include <string>

int precedence_operator(char c)
{
    if(c == '/' || c == '*')
        return 2;
    else if(c == '+' || c == '-')
        return 1;
    else
        return -1;
}


std::string infixToPostfix(std::string &infix)
{
    std::stack<char> _stack_;
    std::string reval;

    for(int i = 0; i < infix.length(); i++)
    {
        char temp_c = infix[i];
        
        if (temp_c >= '0' && temp_c <= '9')
            reval += temp_c;

        else if(temp_c == '(')
            _stack_.push('(');
        else if(temp_c == ')')
        {
            while(!_stack_.empty() && _stack_.top() != '(')
            {
                reval += _stack_.top();
                _stack_.pop();
            }
            _stack_.pop();
        }
        else
        {
            while(!_stack_.empty() && _stack_.top() != '(' &&
                    (precedence_operator(_stack_.top()) > precedence_operator(temp_c) || precedence_operator(_stack_.top()) == precedence_operator(temp_c)
                ))
                {
                    reval += _stack_.top();
                    _stack_.pop();
                }
                _stack_.push(temp_c);
        }
    }

    while(!_stack_.empty())
    {
        reval += _stack_.top();
        _stack_.pop();
    }

    return reval;
}

int computePostfix(std::string postfix)
{

}