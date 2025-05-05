#include "../../include/Kanban/task.h"
#include "../../include/Kanban/kanban_utils.h"
#include <iostream>

using namespace std;

Task::Task()
{
    id = 0;
    title = "";
    description = "";
    dueDate = "";
    status = "To Do";
}

Task::Task(int taskId, string taskTitle, string taskDesc, string taskDate, string taskStatus)
{
    id = taskId;
    title = taskTitle;
    description = taskDesc;
    dueDate = taskDate;
    status = taskStatus;
}

int Task::getId() const
{
    return id;
}

string Task::getTitle() const
{
    return title;
}

string Task::getDescription() const
{
    return description;
}

string Task::getDueDate() const
{
    return dueDate;
}

string Task::getStatus() const
{
    return status;
}

void Task::setId(int taskId)
{
    id = taskId;
}

void Task::setTitle(string taskTitle)
{
    title = taskTitle;
}

void Task::setDescription(string taskDesc)
{
    description = taskDesc;
}

void Task::setDueDate(string taskDate)
{
    dueDate = taskDate;
}

void Task::setStatus(string taskStatus)
{
    status = taskStatus;
}

void Task::display() const
{
    string colorCode = KanbanUtils::colorByStatus(status);
    string resetCode = KanbanUtils::resetColor();

    cout << "\n\tTask ID: " << id;
    cout << "\n\tStatus: " << colorCode << status << resetCode;
    cout << "\n\tTitle: " << colorCode << title << resetCode << "\tDue Date: " << KanbanUtils::getFormattedDate(dueDate);
    cout << "\n\tDescription: " << description << endl;
}