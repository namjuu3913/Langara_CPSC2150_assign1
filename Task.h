/*
Name: JunHyeok Choi
ID:100421921
Instructor: Bita Shadgar
Section: 001
*/
#pragma once
#include <string>
#include <iostream>

class Task
{
public:
    /*
    string_info[] = {task_id, description, dependency}
    integer_info[] = {priority, deadline}

    I used dynamic memory allocation to prevent from the stack overflow when the size of the Task classes is larger then the size of stack.
    */
    Task(const std::string task_id, const std::string description, 
            const int priority, const int deadline, const std::string dependency) 
    {
        this->task_id     = new std::string(task_id);
        this->description = new std::string(description);
        this->dependency  = new std::string(dependency);
        this->priority    = new std::string(priority);
        this->deadline    = new int(deadline);
    }
    Task(const Task& other) 
    {
        task_id     = new std::string(*other.task_id);
        description = new std::string(*other.description);
        dependency  = new std::string(*other.dependency);
        priority    = new int(*other.priority);
        deadline    = new int(*other.deadline);
    }
    Task& operator=(const Task& other) 
    {
        if (this == &other) 
            return *this;

        *task_id     = *other.task_id;
        *description = *other.description;
        *dependency  = *other.dependency;
        *priority    = *other.priority;  
        *deadline    = *other.deadline;  

        return *this;
    }
    ~Task() 
    {
        cleanup();
    }

    //getter
    const std::string get_task_id() const { return *task_id; }
    const std::string get_description() const { return *description; }
    const std::string get_dependency() const { return *dependency; }
    int get_priority() const { return *priority; }
    int get_deadline() const { return *deadline; }
    
    //toString
    std::string toString() const
    {
        return ("Task info:\n \tTask id: " + *task_id + "\n\tDescription: " + *description + "\n\tPriority: " + std::to_string(*priority) + "\n\tDeadline: " + std::to_string(*deadline) + "\n\tDependency: " + *dependency + "\n");
    }
    //ostream
    friend std::ostream& operator<<(std::ostream& os, const Task& t) 
    {
        os << t.toString();
        return os;
    }


private:
    void cleanup() 
    {
        delete task_id; delete description; delete dependency;
        delete priority; delete deadline;
    }
    //string_info[]
    std::string *task_id = nullptr;        //Unique identifier
    std::string *description = nullptr;    //Task description
    std::string *dependency = nullptr;     //ID of another task this task depends on (empty if none)
    //integer_info[]
    int *priority = nullptr;
    int *deadline = nullptr;
};

