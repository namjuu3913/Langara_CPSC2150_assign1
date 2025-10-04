#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template <typename T>
class QueueNode 
{
public:
    T data;
    QueueNode<T>* next;
    QueueNode(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Queue 
{
private:
    QueueNode<T>* frontNode;
    QueueNode<T>* rearNode;
    int count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~Queue() 
    {
        while (!isEmpty()) 
        {
            dequeue();
        }
    }

    void enqueue(T val) 
    {
        QueueNode<T>* newNode = new QueueNode<T>(val);
        if (isEmpty()) 
        {
            frontNode = rearNode = newNode;
        } 
        else 
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    T dequeue() 
    {
        if (isEmpty()) 
            throw std::runtime_error("Queue is empty");

        QueueNode<T>* temp = frontNode;
        T val = temp->data;
        frontNode = frontNode->next;

        if (frontNode == nullptr) 
            rearNode = nullptr;

        delete temp;
        count--;
        return val;
    }

    T front() 
    {
        if (isEmpty()) 
            throw std::runtime_error("Queue is empty");
        return frontNode->data;
    }
    
    bool isEmpty() 
    {
        return frontNode == nullptr;
    }
    
    int size() 
    {
        return count;
    }
    
    void display() const 
    {
        QueueNode<T>* current = frontNode;
        while(current != nullptr) 
        {
            current->data.display();
            current = current->next;
        }
    }
};
#endif