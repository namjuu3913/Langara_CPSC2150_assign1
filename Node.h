
#pragma once

template <typename T>
class Node
{
public:
    Node(const T& val)
    {
        this->data = val;
        this->prev = nullptr;
        this->next = nullptr;
    }
    
    T data;
    Node* prev;
    Node* next;
};
