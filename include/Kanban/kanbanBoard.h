#ifndef KANBANBOARD_H
#define KANBANBOARD_H

#include "task.h"
#include "kanbanFileManager.h" 
#include <vector>
#include <string>

using namespace std;

class KanbanBoard
{
private:
    vector<Task> tasks;
    int nextId;
    vector<int> deletedIds;
    KanbanFileManager fileManager;

    void updateNextId();

public:
    KanbanBoard(const string& username);
    ~KanbanBoard();

    void addTask(const string& title, const string& description, const string& dueDate);
    bool updateTask(int id, const string& title, const string& description, const string& dueDate, const string& status);
    bool deleteTask(int id);
    bool moveTask(int id, const string& newStatus);
    vector<Task> searchTasks(const string& searchTerm) const;

    Task* findTaskById(int id);
    bool isEmpty() const { return tasks.empty(); }
    void getTasksByStatus(vector<Task>& todoTasks, vector<Task>& inProgressTasks, vector<Task>& doneTasks) const;
};

#endif