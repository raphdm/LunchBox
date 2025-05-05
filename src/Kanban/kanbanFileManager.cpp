#include "../../include/Kanban/KanbanFileManager.h"
#include "../../include/Kanban/kanban_utils.h" 
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <algorithm> 
#include <cstdio> 
#include <cerrno> 

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <unistd.h>
#include <sys/types.h> 
#define MKDIR(dir) mkdir(dir, 0777) 
#endif

using namespace std;

KanbanFileManager::KanbanFileManager(const string& username)
    : username(username)
{
    if (username.empty()) {
        cerr << "Critical Error: Username cannot be empty for KanbanFileManager." << endl;
        userKanbanDir = BASE_KANBAN_DIR + "INVALID_USER/";
    }
    else {
        userKanbanDir = BASE_KANBAN_DIR + username + "/";
    }
    ensureDirectoryExists();
}

bool KanbanFileManager::directoryExists(const string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false; // Cannot access path
    }
    return (info.st_mode & S_IFDIR) != 0; // Check if it's a directory
}

void KanbanFileManager::ensureDirectoryExists()
{
    // 1. Ensure SavedData exists
    if (!directoryExists("SavedData"))
    {
        if (MKDIR("SavedData") != 0 && errno != EEXIST)
        {
            cerr << "Warning: Failed to create SavedData directory. Error: " << errno << endl;
            return; // Stop if base SavedData cannot be created
        }
    }

    // 2. Ensure SavedData/Kanban exists
    if (!directoryExists(BASE_KANBAN_DIR))
    {
        if (MKDIR(BASE_KANBAN_DIR.c_str()) != 0 && errno != EEXIST)
        {
            cerr << "Warning: Failed to create " << BASE_KANBAN_DIR << " directory. Error: " << errno << endl;
            return; // Stop if base Kanban dir cannot be created
        }
    }

    // 3. Ensure SavedData/Kanban/<username> exists
    if (!userKanbanDir.empty() && !directoryExists(userKanbanDir))
    {
        if (MKDIR(userKanbanDir.c_str()) != 0 && errno != EEXIST)
        {
            cerr << "Warning: Failed to create user directory " << userKanbanDir << ". Error: " << errno << endl;
        }
    }
}

void KanbanFileManager::saveTasks(const vector<Task>& tasks) const
{
    if (username.empty()) return;
    string filePath = userKanbanDir + "tasks.txt";
    ofstream taskFile(filePath.c_str());
    if (taskFile.is_open())
    {
        for (size_t i = 0; i < tasks.size(); i++)
        {
            if (i > 0) taskFile << "\n";
            taskFile << tasks[i].getId();
            taskFile << "\n" << tasks[i].getStatus();
            taskFile << "\n" << tasks[i].getTitle();
            taskFile << "\n" << tasks[i].getDueDate();
            taskFile << "\n" << tasks[i].getDescription();
        }
        taskFile.close();
    }
    else
    {
        cerr << "Error: Could not save tasks to file: " << filePath << endl;
    }
}

vector<Task> KanbanFileManager::loadTasks(int& nextId, vector<int>& deletedIds)
{
    vector<Task> loadedTasks;
    if (username.empty()) {
        nextId = 1;
        deletedIds.clear();
        return loadedTasks; // Return empty if no username
    }

    string filePath = userKanbanDir + "tasks.txt";
    ifstream taskFile(filePath.c_str());

    if (taskFile.is_open())
    {
        loadedTasks.clear();
        string line;
        Task currentTask;
        int fieldCounter = 0;

        while (getline(taskFile, line))
        {
            if (line.empty() && taskFile.peek() == EOF) break;

            switch (fieldCounter % 5)
            {
            case 0:
                try {
                    currentTask.setId(stoi(line));
                }
                catch (...) {
                    cerr << "Warning: Invalid ID format in tasks file for user '" << username << "': " << line << endl;
                    currentTask.setId(0);
                }
                break;
            case 1: currentTask.setStatus(line); break;
            case 2: currentTask.setTitle(line); break;
            case 3: currentTask.setDueDate(line); break;
            case 4:
                currentTask.setDescription(line);
                if (currentTask.getId() != 0)
                {
                    loadedTasks.push_back(currentTask);
                }
                currentTask = Task();
                break;
            }
            fieldCounter++;
        }
        taskFile.close();
    }
    else
    {
        // Only print message if file doesn't exist, not if dir creation failed earlier
        struct stat buffer;
        if (stat(filePath.c_str(), &buffer) != 0) {
            cout << "No existing Kanban data found for user '" << username << "'. Starting fresh." << endl;
        }
        else {
            cerr << "Error: Could not open tasks file: " << filePath << endl;
        }
    }

    nextId = loadId();
    deletedIds = loadDeletedIds();

    return loadedTasks;
}


void KanbanFileManager::saveId(int nextId) const
{
    if (username.empty()) return;
    string filePath = userKanbanDir + "id.txt";
    ofstream idFile(filePath.c_str());
    if (idFile.is_open())
    {
        idFile << nextId;
        idFile.close();
    }
    else {
        cerr << "Error: Could not save ID to file: " << filePath << endl;
    }
}

int KanbanFileManager::loadId()
{
    int loadedId = 1;
    if (username.empty()) return loadedId;

    string filePath = userKanbanDir + "id.txt";
    ifstream idFile(filePath.c_str());
    if (idFile.is_open())
    {
        if (!(idFile >> loadedId)) {
            loadedId = 1; // Default if file is empty or corrupt
        }
        idFile.close();
    }
    // No error message if file doesn't exist, just use default ID 1
    return loadedId;
}

void KanbanFileManager::saveDeletedIds(const vector<int>& deletedIds) const
{
    if (username.empty()) return;
    string filePath = userKanbanDir + "deleted_ids.txt";
    ofstream deletedIdsFile(filePath.c_str());
    if (deletedIdsFile.is_open()) {
        for (size_t i = 0; i < deletedIds.size(); i++)
        {
            if (i > 0) deletedIdsFile << "\n";
            deletedIdsFile << deletedIds[i];
        }
        deletedIdsFile.close();
    }
    else {
        cerr << "Error: Could not save deleted IDs to file: " << filePath << endl;
    }
}

vector<int> KanbanFileManager::loadDeletedIds()
{
    vector<int> loadedDeletedIds;
    if (username.empty()) return loadedDeletedIds;

    string filePath = userKanbanDir + "deleted_ids.txt";
    ifstream deletedIdsFile(filePath.c_str());
    if (deletedIdsFile.is_open()) {
        int id;
        while (deletedIdsFile >> id) {
            loadedDeletedIds.push_back(id);
        }
        deletedIdsFile.close();
        sort(loadedDeletedIds.begin(), loadedDeletedIds.end());
    }
    // No error message if file doesn't exist, just return empty vector
    return loadedDeletedIds;
}