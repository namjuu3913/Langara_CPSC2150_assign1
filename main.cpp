#include <iostream>
#include <limits>
#include "TaskScheduler.h"

void displayMenu() 
{
    std::cout << "\n===== Task Scheduler Menu =====\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. Delete Task\n";
    std::cout << "3. Display All Tasks\n";
    std::cout << "4. Schedule Next Task\n";
    std::cout << "5. Check Cyclic Dependencies\n";
    std::cout << "6. Save Completed Tasks to File\n";
    std::cout << "7. Load Tasks from File\n";
    std::cout << "8. Exit\n";
    std::cout << "=============================\n";
    std::cout << "Enter choice: ";
}

void clearInputBuffer() 
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() 
{
    TaskScheduler scheduler;
    int choice;

    do {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) 
        {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();


        switch (choice) 
        {
            case 1: 
            {
                std::string id, desc, priority, dependency;
                int deadline;
                std::cout << "Enter Task ID (e.g., T001): ";
                std::getline(std::cin, id);
                std::cout << "Enter Description: ";
                std::getline(std::cin, desc);
                std::cout << "Enter Priority (e.g., 5 or '2 + 1 * priority of T001'): ";
                std::getline(std::cin, priority);
                std::cout << "Enter Deadline (in hours): ";
                std::cin >> deadline;
                clearInputBuffer();
                std::cout << "Enter Dependency ID (or leave blank): ";
                std::getline(std::cin, dependency);
                scheduler.addTask(id, desc, priority, deadline, dependency);
                break;
            }
            case 2: 
            {
                std::string id;
                std::cout << "Enter Task ID to delete: ";
                std::getline(std::cin, id);
                scheduler.deleteTask(id);
                break;
            }
            case 3: 
            {
                scheduler.displayAllTasks();
                break;
            }
            case 4: 
            {
                scheduler.scheduleNextTask();
                break;
            }
            case 5: 
            {
                scheduler.checkCyclicDependencies();
                break;
            }
            case 6: 
            {
                scheduler.saveCompletedTasksToFile("completed_tasks.txt");
                break;
            }
            case 7: 
            {
                scheduler.loadTasksFromFile("tasks.txt");
                break;
            }
            case 8:
                std::cout << "Exiting program." << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 8);

    return 0;
}