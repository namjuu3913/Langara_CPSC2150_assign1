#include "NodeList.h"
#include "Task.h"
#include <string>

class TaskIterator
{
private:
    Node* currentNode;
    Node* listHead;

public:
    TaskIterator(Node* head);
    ~TaskIterator();

    bool hasNext();
    Task* next();
    void reset();
};

TaskIterator::TaskIterator(Node* head)
{
    this->listHead = head;
    this->currentNode = head;
}
TaskIterator::~TaskIterator()
{
    delete currentNode;
    delete listHead;
}
bool TaskIterator::hasNext()
{
    return this->currentNode != nullptr;
}
Task* TaskIterator::next()
{
    if(!hasNext())
    {
        return nullptr;
    }

    Task* taskData = currentNode->data;
    currentNode = currentNode->next;

    return taskData;
}
void TaskIterator::reset()
{
    this->currentNode = this->listHead;
}