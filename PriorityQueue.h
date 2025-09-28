#include "CustomLinkedList.h"
#include "Task.h"
#include "TaskIterator.h"

class PriQueue
{
public:
    PriQueue();
    PriQueue(const PriQueue& other);
    PriQueue& operator=(const PriQueue& other);
    ~PriQueue();

    int getSize();
    bool isNmpty();
    void insert(Task task);
    Task getMin();
    void removeMin();
    void changeQueue();

private:
    CustomLinkedList* list;
};

PriQueue::PriQueue()
{
    
}