/*
Name: JunHyeok Choi
ID:100421921
Instructor: Bita Shadgar
Section: 001
*/
#ifndef TASK_H
#define TASK_H

#include <string>
#include <iostream>

class Task {
public:
    std::string taskID;
    std::string description;
    std::string priorityExpression; // e.g., "5" or "2 + 1.5 * priority of T002"
    int calculatedPriority;
    int deadline;
    std::string dependency;

    // Constructor
    Task(std::string id = "", std::string desc = "", std::string priorityExpr = "10", int dl = 0, std::string dep = "");

    // Method to display task details
    void display() const;
};

Task::Task(std::string id, std::string desc, std::string priorityExpr, int dl, std::string dep)
    : taskID(id), description(desc), priorityExpression(priorityExpr), calculatedPriority(10), deadline(dl), dependency(dep) {}

void Task::display() const 
{
    std::cout << "  - ID: " << taskID
              << ", Desc: " << description
              << ", Priority: " << calculatedPriority
              << " (Expr: \"" << priorityExpression << "\")"
              << ", Deadline: " << deadline << " hours"
              << ", Depends on: " << (dependency.empty() ? "None" : dependency)
              << std::endl;
}

#endif