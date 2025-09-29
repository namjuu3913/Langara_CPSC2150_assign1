/*
Name: JunHyeok Choi
ID:100421921
Instructor: Bita Shadgar
Section: 001
*/
#pragma once
#include "Task.h"
#include <string>
#include "TaskIterator.h"
#include "Node.h"

class ListedPQueue
{
public:
    ListedPQueue(bool isSorted);
    ListedPQueue(const ListedPQueue& other);
    ListedPQueue& operator=(const ListedPQueue& other);
    ~ListedPQueue();
    
    void insertTask(Task task);
    void deleteTask(std::string taskID);
    void deleteMin();
    Task findTask(std::string taskID) const;
    Task getMin();
    Task getMax();

    TaskIterator getTaskIterator();
private:
    void sort();
    void mergeSort(Node** headPtr);
    Node* merge(Node* a, Node* b);
    void splitList(Node* source, Node** frontPtr, Node** backPtr);

    Node* head;
    Node* tail; 
    
    bool isSorted;
};

ListedPQueue::ListedPQueue(bool isSorted)
{
    this->isSorted = isSorted;
    this->head = nullptr;
    this->tail = nullptr;
}
ListedPQueue::ListedPQueue(const ListedPQueue& other)
{
    this->head = nullptr;
    this->tail = nullptr;
    
    for(Node* temp_current = other.head; temp_current != nullptr; temp_current = temp_current->next)
    {
        Node* new_node = new Node(*(temp_current->data));

        if (this->head == nullptr)
        {
            this->head = new_node;
            this->tail = new_node;
        }
        else
        {
            this->tail->next = new_node;
            new_node->prev = this->tail;
            this->tail = new_node;
        }
    }
}
ListedPQueue& ListedPQueue::operator=(const ListedPQueue& other)
{
    if(this == &other)
    {
        return *this;
    }

    Node* temp_del = this->head;
    for(;temp_del != nullptr;)
    {
        Node* next_node = temp_del->next;
        delete temp_del;
        temp_del = next_node;
    }

    this->head = nullptr;
    this->tail = nullptr;
    
    for(Node* temp_current = other.head; temp_current != nullptr; temp_current = temp_current->next)
    {
        Node* new_node = new Node(*(temp_current->data));

        if (this->head == nullptr)
        {
            this->head = new_node;
            this->tail = new_node;
        }
        else
        {
            this->tail->next = new_node;
            new_node->prev = this->tail;
            this->tail = new_node;
        }
    }

    return *this;
}
ListedPQueue::~ListedPQueue()
{
    Node* temp_del = this->head;
    for(;temp_del != nullptr;)
    {
        Node* next_node = temp_del->next;
        delete temp_del;
        temp_del = next_node;
    }
}

void ListedPQueue::insertTask(Task task)
{
    Node* new_node = new Node(task);
    if (this->head == nullptr) 
    {
        this->head = new_node;
        this->tail = new_node;
    } 
    else 
    {
        this->tail->next = new_node;
        new_node->prev = this->tail;
        this->tail = new_node;
    }

    if(this->isSorted)
        this->sort();
}
void ListedPQueue::deleteMin()
{
    if (this->head == nullptr) 
    {
        throw "List is empty.";
    }
    if (this->isSorted)
    {
        Node* temp = this->head;   

        if (this->head == this->tail)
        {
            this->head = nullptr;
            this->tail = nullptr;   
        }
        else
        {
            this->head = this->head->next;
            this->head->prev = nullptr;
        }
        delete temp;
    }
    else
    {
        Task temp = this->getMin();
        this->deleteTask(temp.get_task_id());
    }
}
void ListedPQueue::deleteTask(std::string taskID)
{
    Node* temp = this->head;
    while (temp != nullptr && temp->data->get_task_id() != taskID)
    {
        temp = temp->next;
    }

    if(temp == nullptr)
    {
        throw (taskID + "cannot be found in the list.");
    }
    //head
    if(temp->prev != nullptr)
    {
        temp->prev->next = temp->next;
    }
    else
    {
        this->head = temp->next;
    }
    //tail
    if(temp->next != nullptr)
    {
        temp->next->prev = temp->prev;
    }
    else
    {
        this->tail = temp->prev;
    }

    delete temp;
}
Task ListedPQueue::getMin()
{
    if (this->head == nullptr) 
    {
        throw "List is empty.";
    }

    if (this->isSorted)
        return *(this->head->data);
    
    Node* min_node = this->head;
    Node* temp = this->head->next;
    while (temp != nullptr)
    {
        if (min_node->data->get_priority() > temp->data->get_priority() ||
           (min_node->data->get_priority() == temp->data->get_priority() && min_node->data->get_deadline() > temp->data->get_deadline()))
        {
            min_node = temp;
        }
        temp = temp->next;
    }

    return *(min_node->data);
}
Task ListedPQueue::getMax()
{
    if (this->head == nullptr) 
    {
        throw "List is empty.";
    }

    if (this->isSorted)
        return *(this->tail->data);

    Node* max_node = this->head;
    Node* temp = this->head->next;
    while (temp != nullptr)
    {
        if (max_node->data->get_priority() < temp->data->get_priority() ||
           (max_node->data->get_priority() == temp->data->get_priority() && max_node->data->get_deadline() < temp->data->get_deadline()))
        {
            max_node = temp;
        }
        temp = temp->next;
    }
    return *(max_node->data);
}
Task ListedPQueue::findTask(std::string taskID) const 
{
    for (Node* cur = head; cur != nullptr; cur = cur->next) 
    {
        if (cur->data->get_task_id() == taskID) 
            return *(cur->data);
    }
    throw std::string("Task ") + taskID + " not found";
}
TaskIterator ListedPQueue::getTaskIterator()
{
    return TaskIterator(this->head);
}

//merge sort
void ListedPQueue::sort()
{
    mergeSort(&this->head);

    if(this->head == nullptr)
    {
        this->tail = nullptr;
    }
    else
    {
        head->prev = nullptr;
        Node* temp = this->head;
        for(;temp->next != nullptr;)
        {
            temp = temp->next;
        }
        this->tail = temp;
    }
}
void ListedPQueue::mergeSort(Node** headPtr)
{
    Node* head = *headPtr;
    Node* leftHalf;
    Node* rightHalf;

    //base case: if list is empty or there is only one node, it is already sorted.
    if(head == nullptr || head->next == nullptr)
    {
        return;
    }
    //split the list
    splitList(head, &leftHalf, &rightHalf);
    //sort these parts with recursion
    mergeSort(&leftHalf);
    mergeSort(&rightHalf);

    *headPtr = merge(leftHalf, rightHalf);
}
void ListedPQueue::splitList(Node* source, Node** frontPtr, Node** backPtr)
{
    Node* high;
    Node* low;
    low = source;
    high = source->next;

    while(high != nullptr)
    {
        high = high->next;
        if(high != nullptr)
        {
            low = low->next;
            high = high->next;
        }
    }

    *frontPtr = source;
    *backPtr = low->next;
    low->next = nullptr;

    if (*backPtr != nullptr) 
    {
        (*backPtr)->prev = nullptr;
    }
}
Node* ListedPQueue::merge(Node* a, Node* b)
{
    // Base cases
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    Node* result = nullptr;

    if (a->data->get_priority() < b->data->get_priority() ||
        (a->data->get_priority() == b->data->get_priority() && a->data->get_deadline() <= b->data->get_deadline()))
    {
        result = a;
        Node* mergedTail = merge(a->next, b);
        result->next = mergedTail;

        if (mergedTail != nullptr) 
        {
            mergedTail->prev = result;
        }
    }
    else
    {
        result = b;
        Node* mergedTail = merge(a, b->next);
        result->next = mergedTail;

        if (mergedTail != nullptr) 
        {
            mergedTail->prev = result;
        }
    }

    return result;
}
