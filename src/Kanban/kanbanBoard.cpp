#include "../../include/Kanban/kanbanBoard.h"
#include "../../include/Kanban/kanban_utils.h"
#include <algorithm>
#include <iostream>

using namespace std;

KanbanBoard::KanbanBoard(const string& username) : fileManager(username)
{
    tasks = fileManager.loadTasks(nextId, deletedIds);
    updateNextId();
}

KanbanBoard::~KanbanBoard()
{
    fileManager.saveTasks(tasks);
    fileManager.saveId(nextId);
    fileManager.saveDeletedIds(deletedIds);
}

void KanbanBoard::updateNextId()
{
    int maxId = 0;
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        if (tasks[i].getId() > maxId)
        {
            maxId = tasks[i].getId();
        }
    }

    if (maxId >= nextId)
    {
        nextId = maxId + 1;
        fileManager.saveId(nextId);
    }
}

Task* KanbanBoard::findTaskById(int id)
{
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].getId() == id) {
            return &tasks[i];
        }
    }
    return nullptr;
}

void KanbanBoard::addTask(const string& title, const string& description, const string& dueDate)
{
    Task task;
    int assignedId;

    if (!deletedIds.empty()) {
        assignedId = deletedIds.front();
        deletedIds.erase(deletedIds.begin());
        fileManager.saveDeletedIds(deletedIds);
    }
    else
    {
        assignedId = nextId++;
        fileManager.saveId(nextId);
    }

    task.setId(assignedId);
    task.setTitle(title);
    task.setDescription(description);
    task.setDueDate(dueDate);
    task.setStatus("To Do");

    tasks.push_back(task);
    fileManager.saveTasks(tasks);
}

bool KanbanBoard::updateTask(int id, const string& title, const string& description, const string& dueDate, const string& status)
{
    Task* task = findTaskById(id);
    if (task == nullptr) {
        return false;
    }

    bool changed = false;

    if (!title.empty() && task->getTitle() != title) {
        task->setTitle(title);
        changed = true;
    }

    if (!description.empty() && task->getDescription() != description) {
        task->setDescription(description);
        changed = true;
    }

    if (!dueDate.empty()) {
        if (KanbanUtils::isValidDate(dueDate)) {
            if (task->getDueDate() != dueDate) {
                task->setDueDate(dueDate);
                changed = true;
            }
        }
        else {
            cout << "Warning: Invalid due date format provided during update. Due date not changed." << endl;
        }
    }


    if (!status.empty() && task->getStatus() != status) {
        if (status == "To Do" || status == "In Progress" || status == "Done") {
            task->setStatus(status);
            changed = true;
        }
        else {
            cout << "Warning: Invalid status provided during update. Status not changed." << endl;
        }
    }

    if (changed) {
        fileManager.saveTasks(tasks);
    }
    return true;
}

bool KanbanBoard::deleteTask(int id)
{
    for (vector<Task>::iterator it = tasks.begin(); it != tasks.end(); ++it)
    {
        if (it->getId() == id)
        {
            bool alreadyDeleted = false;
            for (size_t i = 0; i < deletedIds.size(); ++i) {
                if (deletedIds[i] == id) {
                    alreadyDeleted = true;
                    break;
                }
            }

            if (!alreadyDeleted)
            {
                deletedIds.push_back(id);
                sort(deletedIds.begin(), deletedIds.end());
                fileManager.saveDeletedIds(deletedIds);
            }
            tasks.erase(it);
            fileManager.saveTasks(tasks);
            return true;
        }
    }
    return false;
}

bool KanbanBoard::moveTask(int id, const string& newStatus)
{
    Task* task = findTaskById(id);
    if (task != nullptr)
    {
        if (newStatus == "To Do" || newStatus == "In Progress" || newStatus == "Done")
        {
            if (task->getStatus() != newStatus) {
                task->setStatus(newStatus);
                fileManager.saveTasks(tasks);
            }
            return true;
        }
        else {
            cout << "Warning: Invalid status provided for move operation. Task not moved." << endl;
            return false;
        }
    }
    return false;
}

vector<Task> KanbanBoard::searchTasks(const string& searchTerm) const
{
    vector<Task> results;
    if (searchTerm.empty()) return results;

    string lowerSearchTerm = searchTerm;
    transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        string title = tasks[i].getTitle();
        string desc = tasks[i].getDescription();

        string lowerTitle = title;
        string lowerDesc = desc;
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(), ::tolower);

        if (lowerTitle.find(lowerSearchTerm) != string::npos ||
            lowerDesc.find(lowerSearchTerm) != string::npos)
        {
            results.push_back(tasks[i]);
        }
    }

    return results;
}

void KanbanBoard::getTasksByStatus(vector<Task>& todoTasks, vector<Task>& inProgressTasks, vector<Task>& doneTasks) const
{
    todoTasks.clear();
    inProgressTasks.clear();
    doneTasks.clear();

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        if (tasks[i].getStatus() == "To Do")
        {
            todoTasks.push_back(tasks[i]);
        }
        else if (tasks[i].getStatus() == "In Progress")
        {
            inProgressTasks.push_back(tasks[i]);
        }
        else if (tasks[i].getStatus() == "Done")
        {
            doneTasks.push_back(tasks[i]);
        }
    }

    auto sortByDueDate = [](const Task& a, const Task& b)
        {
            // Handle potential empty dates if necessary, placing them last/first
            if (a.getDueDate().empty() && !b.getDueDate().empty()) return false;
            if (!a.getDueDate().empty() && b.getDueDate().empty()) return true;
            return a.getDueDate() < b.getDueDate();
        };

    sort(todoTasks.begin(), todoTasks.end(), sortByDueDate);
    sort(inProgressTasks.begin(), inProgressTasks.end(), sortByDueDate);
    sort(doneTasks.begin(), doneTasks.end(), sortByDueDate);
}