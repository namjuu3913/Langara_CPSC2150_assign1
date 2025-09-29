#pragma once
#include "Task.h"

class Node
{
public:
    Node(const Task& val)
    {
        this->data = new Task(val);
        this->prev = nullptr;
        this->next = nullptr;
    }
    ~Node()
    {
        delete data;
    }
    
    Task* data;
    Node* prev;
    Node* next;
};
