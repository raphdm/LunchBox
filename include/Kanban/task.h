#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

using namespace std;

class Task
{
private:
    int id;
    string title;
    string description;
    string dueDate;
    string status;

public:
    Task();
    Task(int taskId, string taskTitle, string taskDesc, string taskDate, string taskStatus = "To Do");

    int getId() const;
    string getTitle() const;
    string getDescription() const;
    string getDueDate() const;
    string getStatus() const;

    void setId(int taskId);
    void setTitle(string taskTitle);
    void setDescription(string taskDesc);
    void setDueDate(string taskDate);
    void setStatus(string taskStatus);

    void display() const;
};

#endif