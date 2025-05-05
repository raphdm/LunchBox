#ifndef KANBANFILEMANAGER_H
#define KANBANFILEMANAGER_H

#include "task.h"
#include <vector>
#include <string>
#include <fstream> 

using namespace std;

class KanbanFileManager
{
private:
    const string BASE_KANBAN_DIR = "SavedData/Kanban/";
    string userKanbanDir;
    string username;

    bool directoryExists(const string& path);
    void ensureDirectoryExists();

public:
    KanbanFileManager(const string& username);

    void saveTasks(const vector<Task>& tasks) const;
    vector<Task> loadTasks(int& nextId, vector<int>& deletedIds);
    void saveId(int nextId) const;
    int loadId();
    void saveDeletedIds(const vector<int>& deletedIds) const;
    vector<int> loadDeletedIds();

    string getUserKanbanDir() const { return userKanbanDir; }
};

#endif