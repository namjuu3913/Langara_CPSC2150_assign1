#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Task.h"
#include "ListedPQueue.h"
#include "TaskIterator.h"

struct RawTaskRow {
    std::string id;
    std::string desc;
    std::string priorityOrExpr;
    int         deadline;
    std::string dependency;
};

inline std::vector<RawTaskRow> LoadRawTasksFromFile(const std::string& path);
inline void SaveTasksToFile(const std::string& path, const std::vector<Task>& tasks);


static std::string trim(const std::string& str) 
{
    size_t b = 0, e = str.size();
    while (b < e && std::isspace(static_cast<unsigned char>(str[b]))) 
        ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(str[e - 1]))) 
        --e;
    return str.substr(b, e - b);
}

static bool split5(const std::string& line,
                   std::string& c0, std::string& c1, std::string& c2,
                   std::string& c3, std::string& c4)
{
    size_t p0 = 0;
    size_t p1 = line.find(',', p0);
    if (p1 == std::string::npos) 
        return false;
    c0 = trim(line.substr(p0, p1 - p0));

    size_t p2 = line.find(',', p1 + 1);
    if (p2 == std::string::npos) 
        return false;
    c1 = trim(line.substr(p1 + 1, p2 - (p1 + 1)));

    size_t p3 = line.find(',', p2 + 1);
    if (p3 == std::string::npos) 
        return false;
    c2 = trim(line.substr(p2 + 1, p3 - (p2 + 1)));

    size_t p4 = line.find(',', p3 + 1);
    if (p4 == std::string::npos) 
        return false;
    c3 = trim(line.substr(p3 + 1, p4 - (p3 + 1)));

    c4 = trim(line.substr(p4 + 1)); // 나머지 전부
    return true;
}

inline std::vector<RawTaskRow> LoadRawTasksFromFile(const std::string& path) 
{
    std::ifstream fin(path.c_str());
    if (!fin.is_open()) 
        throw std::runtime_error("Cannot open file: " + path);

    std::vector<RawTaskRow> rows;
    std::string line;
    while (std::getline(fin, line)) 
    {
        std::string t = trim(line);

        if (t.empty() || t[0] == '#') 
            continue;

        std::string s0, s1, s2, s3, s4;
        if (!split5(line, s0, s1, s2, s3, s4)) 
        {
            throw std::runtime_error("Bad row (need 5 fields): " + line);
        }

        // deadline
        int deadline = 0;
        {
            bool neg = false; size_t i = 0; long val = 0;

            if (!s3.empty() && (s3[0]=='-'||s3[0]=='+')) 
            { 
                neg = (s3[0]=='-'); i=1; 
            }
            for (; i < s3.size(); ++i) 
            {
                if (!std::isdigit(static_cast<unsigned char>(s3[i])))
                    throw std::runtime_error("Deadline must be integer: " + s3);
                val = val * 10 + (s3[i]-'0');
            }
            deadline = static_cast<int>(neg ? -val : val);
        }

        RawTaskRow r;
        r.id             = s0;
        r.desc           = s1;
        r.priorityOrExpr = s2;   // 숫자 or 수식 문자열
        r.deadline       = deadline;
        r.dependency     = s4;

        rows.push_back(r);
    }
    return rows;
}

inline void SaveTasksToFile(const std::string& path, const std::vector<Task>& tasks) 
{
    std::ofstream fout(path.c_str());
    if (!fout.is_open()) 
        throw std::runtime_error("Cannot write file: " + path);

    for (const Task& t : tasks) {
        fout << t.get_task_id() << ","
             << t.get_description() << ","
             << t.get_priority() << ","
             << t.get_deadline() << ","
             << t.get_dependency() << "\n";
    }
}

static bool isNumber(const std::string& s) 
{
    if (s.empty()) 
        return false;
    size_t i = (s[0]=='-'||s[0]=='+') ? 1 : 0;

    if (i >= s.size()) 
        return false;

    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) 
            return false;
    return true;
}

void LoadTasks_NumberOnly(const std::string& path, ListedPQueue& pq) 
{
    auto rows = LoadRawTasksFromFile(path);
    for (const auto& r : rows) 
    {
        if (!isNumber(r.priorityOrExpr)) 
        {
            throw std::runtime_error("Expression found. Add expression evaluator first: " + r.priorityOrExpr);
        }
        int pr = 0;
        {
            const std::string& s = r.priorityOrExpr;
            bool neg = false; 
            size_t i = 0; 
            long val = 0;

            if (!s.empty() && (s[0]=='-'||s[0]=='+')) 
            { 
                neg = (s[0]=='-'); 
                i=1; 
            }

            for (; i < s.size(); ++i) 
                val = val*10 + (s[i]-'0');

            pr = static_cast<int>(neg ? -val : val);
        }
        pq.insertTask(Task(r.id, r.desc, pr, r.deadline, r.dependency));
    }
}

std::vector<Task> DumpPQToVector(ListedPQueue& pq) 
{
    std::vector<Task> out;
    auto it = pq.getTaskIterator();

    while (it.hasNext()) 
    {
        out.push_back(*(it.next()));
    }

    return out;
}

void SaveAll(const std::string& path, ListedPQueue& pq) 
{
    auto tasks = DumpPQToVector(pq);
    try 
    {
        SaveTasksToFile(path, tasks);
        std::cout << "Saved " << tasks.size() << " tasks to " << path << "\n";
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Save error: " << e.what() << "\n";
    }
}
