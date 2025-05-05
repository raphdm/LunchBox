#ifndef KANBAN_UTILS_H
#define KANBAN_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "task.h"

using namespace std;

namespace KanbanUtils
{
    const int COLUMN_WIDTH = 30;
    const int TERMINAL_WIDTH = 100;

    void clearScreen();
    void displayHeader();
    void displayFooter();

    bool isValidDate(const string& date);
    string getFormattedDate(const string& date);
    string colorByStatus(const string& status);
    string resetColor();

    void displayTasksInColumns(const vector<Task>& todoTasks,
        const vector<Task>& inProgressTasks,
        const vector<Task>& doneTasks);

    string formatTaskForColumn(const Task& task);

    string horizontalLine(char c = '-', int width = TERMINAL_WIDTH);

    string truncateText(const string& text, int maxWidth);

}

#endif