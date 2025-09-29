#include <fstream>
#include "CustomLinkedList.h"
#include "Task.h"
#include "TaskIterator.h"

class TaskScheduler
{
public:
    TaskScheduler(bool isSorted);
    TaskScheduler(const TaskScheduler& other);
    TaskScheduler& operator=(const TaskScheduler& other);
    ~TaskScheduler();

    int getSize();
    bool isNmpty();
    void insert(Task task);
    Task getMin();
    TaskIterator getTaskIterator();
    void removeMin();
    void load();
    void save();

private:
    CustomLinkedList* list;

    int size = 0;
};

TaskScheduler::TaskScheduler(bool isSorted)
{
    list = new CustomLinkedList(bool isSorted);
}
TaskScheduler::TaskScheduler(const TaskScheduler& other)
{
    this->list = other.list;
}
TaskScheduler& TaskScheduler::operator=(const TaskScheduler& other)
{
    if(this == &other)
    {
        return *this;
    }

    delete list;
    list = other.list;
}
TaskScheduler::~TaskScheduler()
{
    delete list;
}

int TaskScheduler::getSize()
{
    return this->size;
}
bool TaskScheduler::isNmpty()
{
    return (this->size == 0);
}
void TaskScheduler::insert(Task task)
{
    this->list->insertTask(task);
    (this->size)++;
}
Task TaskScheduler::getMin()
{
    return this->list->getMin();
}
TaskIterator TaskScheduler::getTaskIterator()
{
    return this->list->getTaskIterator();
}
void TaskScheduler::removeMin()
{
    thit->list->deleteMin();
    (this->size)--;
}
void TaskScheduler::load()
{

}
void TaskScheduler::save()
{

}