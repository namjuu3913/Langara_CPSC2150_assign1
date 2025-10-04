#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "Task.h"
#include <memory> // For std::unique_ptr if needed, but we'll stick to raw pointers for consistency.

// Node for the sorted linked list
template <typename T>
class PQNode 
{
public:
    T data;
    PQNode<T>* next;
    PQNode(T val) : data(val), next(nullptr) {}
};

class PriorityQueue 
{
private:
    PQNode<Task>* head;

public:
    PriorityQueue() : head(nullptr) {}

    ~PriorityQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue task in sorted order
    void enqueue(const Task& task) 
    {
        PQNode<Task>* newNode = new PQNode<Task>(task);
        
        // If list is empty or new node has higher priority than head
        if (head == nullptr || 
            task.calculatedPriority < head->data.calculatedPriority ||
            (task.calculatedPriority == head->data.calculatedPriority && task.deadline < head->data.deadline)) 
        {
            newNode->next = head;
            head = newNode;
        } 
        else 
        {
            PQNode<Task>* current = head;
            // Traverse to find the correct insertion point
            while (current->next != nullptr && 
                   (task.calculatedPriority > current->next->data.calculatedPriority ||
                   (task.calculatedPriority == current->next->data.calculatedPriority && task.deadline >= current->next->data.deadline))) 
            {
                current = current->next;
            }

            newNode->next = current->next;
            current->next = newNode;
        }
    }

    // Dequeue the highest priority task
    Task dequeue() 
    {
        if (isEmpty()) 
        {
            throw std::runtime_error("Priority Queue is empty");
        }
        PQNode<Task>* temp = head;
        Task taskData = temp->data;
        head = head->next;

        delete temp;
        return taskData;
    }

    // Remove a specific task by ID
    bool remove(const std::string& taskId) 
    {
        if (isEmpty()) 
            return false;

        if (head->data.taskID == taskId) 
        {
            PQNode<Task>* temp = head;
            head = head->next;

            delete temp;
            return true;
        }

        PQNode<Task>* current = head;
        while (current->next != nullptr && current->next->data.taskID != taskId)
        {
            current = current->next;
        }

        if (current->next != nullptr) 
        {
            PQNode<Task>* temp = current->next;
            current->next = temp->next;

            delete temp;
            return true;
        }
        return false;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }
};
#endif