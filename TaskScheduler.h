#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "Task.h"
#include "LinkedList.h"
#include "ExpressionEvaluator.h"
#include "PriorityQueue.h"
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <limits>

class TaskScheduler 
{
private:
    LinkedList<Task> allTasks;
    PriorityQueue completedTasks;
    PriorityQueue priorityQueue;

    // Recursive helper for cycle detection
    bool isCyclicUtil(const std::string& taskId, std::set<std::string>& visited, std::set<std::string>& recursionStack);

    // Helper to update the priority queue after a task is completed
    void updateSchedulableTasks(const std::string& completedTaskId);

public:
    TaskScheduler();

    void addTask(const std::string& id, const std::string& desc, const std::string& priorityExpr, int deadline, const std::string& dependency);
    bool deleteTask(const std::string& taskID);
    void displayAllTasks();
    void scheduleNextTask();
    bool checkCyclicDependencies();
    
    void findDependentTasks(const std::string& taskID, std::vector<std::string>& dependents);

    void saveCompletedTasksToFile(const std::string& filename);
    void loadTasksFromFile(const std::string& filename);
};

TaskScheduler::TaskScheduler() {}

void TaskScheduler::addTask(const std::string& id, const std::string& desc, const std::string& priorityExpr, int deadline, const std::string& dependency) {
    if (allTasks.findTask(id)) {
        std::cerr << "Error: Task with ID " << id << " already exists." << std::endl;
        return;
    }
    if (!dependency.empty() && !allTasks.findTask(dependency)) {
        std::cerr << "Error: Dependency task with ID " << dependency << " does not exist." << std::endl;
        return;
    }
    Task newTask(id, desc, priorityExpr, deadline, dependency);
    try {
         newTask.calculatedPriority = ExpressionEvaluator::evaluate(newTask.priorityExpression, &allTasks);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error calculating priority for task " << id << ": " << e.what() << std::endl;
        return;
    }
    
    if (newTask.calculatedPriority < 1 || newTask.calculatedPriority > 10) {
        std::cerr << "Error: Priority must be between 1 and 10." << std::endl;
        return;
    }
    
    allTasks.insert(newTask);

    if (newTask.dependency.empty()) {
        priorityQueue.enqueue(newTask);
    }
    
    std::cout << "Task " << id << " added successfully." << std::endl;
}

bool TaskScheduler::deleteTask(const std::string& taskID) 
{
    Iterator<Task> it = allTasks.begin();
    while(it.hasNext())
    {
        Task& currentTask = it.next();
        if(currentTask.dependency == taskID)
        {
            std::cerr << "Error: Cannot delete task " << taskID << ". Task " << currentTask.taskID << " depends on it." << std::endl;
            return false;
        }
    }

    if (allTasks.remove(taskID)) 
    {
        priorityQueue.remove(taskID);
        std::cout << "Task " << taskID << " deleted." << std::endl;
        return true;
    }
    std::cerr << "Error: Task " << taskID << " not found." << std::endl;
    return false;
}

void TaskScheduler::displayAllTasks() {
    std::cout << "\n--- All Tasks To Be Done ---" << std::endl;
    if (allTasks.isEmpty()) 
    {
        std::cout << "No tasks to display." << std::endl;
    } 
    else 
    {
        Iterator<Task> it = allTasks.begin();
        while (it.hasNext()) 
        {
            it.next().display();
        }
    }
    std::cout << "--------------------------\n" << std::endl;
}

void TaskScheduler::scheduleNextTask() 
{
    if (priorityQueue.isEmpty()) 
    {
        std::cout << "No schedulable tasks available. Check for unmet dependencies or add new tasks." << std::endl;
        return;
    }
    
    Task taskToSchedule = priorityQueue.dequeue();

    std::cout << "Scheduling and completing task:" << std::endl;
    taskToSchedule.display();

    completedTasks.enqueue(taskToSchedule);
    allTasks.remove(taskToSchedule.taskID);


    updateSchedulableTasks(taskToSchedule.taskID);
}

void TaskScheduler::updateSchedulableTasks(const std::string& completedTaskId) 
{
    Iterator<Task> it = allTasks.begin();
    while (it.hasNext()) 
    {
        Task& currentTask = it.next();
        if (currentTask.dependency == completedTaskId) 
        {
            std::cout << "Unlocking task " << currentTask.taskID << " (dependency met)." << std::endl;
            priorityQueue.enqueue(currentTask);
        }
    }
}

// Recursive helper
bool TaskScheduler::isCyclicUtil(const std::string& taskId, std::set<std::string>& visited, std::set<std::string>& recursionStack) {
    visited.insert(taskId);
    recursionStack.insert(taskId);

    Task* task = allTasks.findTask(taskId);
    if (task && !task->dependency.empty()) 
    {
        if (visited.find(task->dependency) == visited.end()) // If dependency not visited
        { 
            if (isCyclicUtil(task->dependency, visited, recursionStack))
                return true;
        } 
        else if (recursionStack.find(task->dependency) != recursionStack.end())  // Found a cycle
        {
            return true;
        }
    }
    recursionStack.erase(taskId);
    return false;
}

bool TaskScheduler::checkCyclicDependencies() {
    std::set<std::string> visited;
    std::set<std::string> recursionStack;
    
    Iterator<Task> it = allTasks.begin();
    while(it.hasNext())
    {
        std::string currentId = it.next().taskID;
        if (visited.find(currentId) == visited.end())
        {
             if(isCyclicUtil(currentId, visited, recursionStack))
             {
                 std::cout << "Error: Cyclic dependency detected involving task " << currentId << std::endl;
                 // As per requirement, break the cycle by removing the task with the least priority
                 // This is a simplified resolution, a real system might need user input.
                 Task* task_to_remove = allTasks.findTask(currentId);
                 int least_priority = -1;

                 // Find the task with the lowest priority in the cycle
                 for(const auto& id : recursionStack)
                 {
                     Task* t = allTasks.findTask(id);
                     if(t && t->calculatedPriority > least_priority)
                     {
                         least_priority = t->calculatedPriority;
                         task_to_remove = t;
                     }
                 }

                 if(task_to_remove)
                 {
                    std::cout << "Breaking cycle by removing task with lowest priority: " << task_to_remove->taskID << std::endl;
                    allTasks.remove(task_to_remove->taskID); // [cite: 105]
                 }
                 return true;
             }
        }
    }
    std::cout << "No cyclic dependencies found." << std::endl;
    return false;
}

void TaskScheduler::findDependentTasks(const std::string& taskID, std::vector<std::string>& dependents) {
    Iterator<Task> it = allTasks.begin();
    while(it.hasNext())
    {
        Task& currentTask = it.next();
        if(currentTask.dependency == taskID)
        {
            dependents.push_back(currentTask.taskID);
            findDependentTasks(currentTask.taskID, dependents); // Recursive call
        }
    }
}


void TaskScheduler::saveCompletedTasksToFile(const std::string& filename) 
{
    std::ofstream outFile(filename);
    if (!outFile) 
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    PriorityQueue tempQueue = completedTasks; // Copy queue to not destroy it

    while(!tempQueue.isEmpty())
    {
        Task t = tempQueue.dequeue();
        outFile << t.taskID << "," << t.description << "," << t.priorityExpression << "," << t.deadline << "," << t.dependency << std::endl;
    }
    std::cout << "Completed tasks saved to " << filename << std::endl;
}

void TaskScheduler::loadTasksFromFile(const std::string& filename) 
{
    std::ifstream inFile(filename);
    if (!inFile) 
    {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inFile, line)) 
    {
        std::stringstream ss(line);
        std::string id, desc, priority, deadline_str, dependency;
        
        std::getline(ss, id, ',');
        std::getline(ss, desc, ',');
        std::getline(ss, priority, ',');
        std::getline(ss, deadline_str, ',');
        std::getline(ss, dependency, ',');

        try 
        {
            int deadline = std::stoi(deadline_str);
            addTask(id, desc, priority, deadline, dependency);
        } 
        catch(const std::invalid_argument& e) 
        {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }
    std::cout << "Tasks loaded from " << filename << std::endl;
}

#endif