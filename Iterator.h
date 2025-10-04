#pragma once
#include "LinkedList.h"
#include "Task.h"
#include <string>
#include "Node.h"

template <typename T>
class Iterator
{
private:
    Node<T>* current;
    Node<T>* start;

public:
    Iterator(Node<T>* startNode) : current(startNode), start(startNode) {}

    bool hasNext() 
    {
        return current != nullptr;
    }

    T& next() {
        if (!hasNext()) 
        {
            throw std::out_of_range("No more elements in the list.");
        }
        T& data = current->data; 
        current = current->next;
        return data;
    }

    void reset() 
    {
        current = start;
    }
};