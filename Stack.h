#pragma once
#include <string>
#include <stdexcept>

template <typename T>
class StackNode 
{
public:
    T data;
    StackNode<T>* next;
    StackNode(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Stack 
{
private:
    StackNode<T>* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() 
    {
        while (!isEmpty()) 
        {
            pop();
        }
    }

    void push(T val) 
    {
        StackNode<T>* newNode = new StackNode<T>(val);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    T pop() {
        if (isEmpty()) 
            throw std::runtime_error("Stack is empty");

        StackNode<T>* temp = topNode;
        T val = temp->data;
        topNode = topNode->next;

        delete temp;

        count--;
        return val;
    }

    T top() 
    {
        if (isEmpty()) 
            throw std::runtime_error("Stack is empty");
        return topNode->data;
    }

    bool isEmpty() 
    {
        return topNode == nullptr;
    }
    
    int size() 
    {
        return count;
    }
};