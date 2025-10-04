# Langara_CPSC2150_assign1
*<b>100421921-JunHyeok Choi*

This project is a C++ simulation of a task scheduler designed to manage tasks with varying priorities, deadlines, and dependencies. It leverages fundamental data structures and algorithms to create a robust, menu-driven command-line application.

## How to Compile and Run

**Prerequisites**
* A C++ compiler that supports the C++11 standard (e.g., g++, Clang).

**Compilation**
1. Place all source files (.h and .cpp) in the same directory.
2. Open a terminal or command prompt in that directory.
3. Compile all the .cpp source files together using the following command:
  ```sh
  g++ main.cpp -std=c++11 -o scheduler
  ```


**Running the Program**
1. After a successful compilation, an executable file named scheduler will be created.
2. Run the program from your terminal:
```sh
./scheduler
```
3. The program will display a menu. Enter the number corresponding to the desired action.

**Loading Tasks from a File**
To use the "Load Tasks from File" feature, create a file named tasks.txt in the same directory. The file should follow this CSV format:

``TaskID,Description,PriorityExpression,Deadline,DependencyID``

Example tasks.txt:

```sh
T001,Write Report,5,24,
T002,Research,2,48,
T003,Review Notes,4,12,T001
T005,Prepare Presentation,1 + priority of T002,18,T003
```
