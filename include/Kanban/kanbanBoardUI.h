#ifndef KANBANBOARDUI_H
#define KANBANBOARDUI_H

#include "kanbanBoard.h"
#include <string>

using namespace std;

class KanbanBoardUI
{
private:
    KanbanBoard& board;

public:
    KanbanBoardUI(KanbanBoard& kanbanBoard);

    void displayMenu() const;
    void displayAllTasks() const;
    void viewTaskDetails() const;
    void handleAddTask();
    void handleUpdateTask();
    void handleDeleteTask();
    void handleMoveTask(const string& newStatus);
    void handleSearchTasks();
    void run();
};

#endif