/*
Name: JunHyeok Choi
ID:100421921
Instructor: Bita Shadgar
Section: 001
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <stdexcept>
#include "Iterator.h"
#include "Node.h"

template <typename T>
class LinkedList 
{
private:
    Node<T>* head;
    Node<T>* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~LinkedList() 
    {
        Node<T>* current = head;
        while (current != nullptr) 
        {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
    }

    // Insert at the end of the list
    void insert(T data) 
    {
        Node<T>* newNode = new Node<T>(data);
        if (head == nullptr) 
        {
            head = tail = newNode;
        } 
        else 
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Find a node by a string identifier
    Node<T>* findNode(const std::string& id) 
    {
        Node<T>* current = head;
        while (current != nullptr) 
        {
            if (current->data.taskID == id) 
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // Find task by ID
    T* findTask(const std::string& id) 
    {
        Node<T>* node = findNode(id);
        if (node) 
        {
            return &(node->data);
        }
        return nullptr;
    }
    

    // Delete a node by a string identifier
    bool remove(const std::string& id) 
    {
        Node<T>* nodeToDelete = findNode(id);
        if (!nodeToDelete) 
            return false;

        if (nodeToDelete == head) 
            head = nodeToDelete->next;
        if (nodeToDelete == tail) 
            tail = nodeToDelete->prev;

        if (nodeToDelete->prev) 
            nodeToDelete->prev->next = nodeToDelete->next;
        if (nodeToDelete->next) 
            nodeToDelete->next->prev = nodeToDelete->prev;

        delete nodeToDelete;
        count--;
        return true;
    }
    
    int size() const 
    {
        return count;
    }

    bool isEmpty() const 
    {
        return head == nullptr;
    }

    // Iterator creation
    Iterator<T> begin() 
    {
        return Iterator<T>(head);
    }
};

#endif