#include "../../include/Kanban/kanbanBoardUI.h"
#include "../../include/Kanban/kanban_utils.h"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

KanbanBoardUI::KanbanBoardUI(KanbanBoard& kanbanBoard) : board(kanbanBoard)
{
}

void KanbanBoardUI::displayMenu() const
{
    KanbanUtils::clearScreen();
    KanbanUtils::displayHeader();

    cout << "\n\t[1] View Kanban Board";
    cout << "\n\t[2] Add New Task";
    cout << "\n\t[3] Update Task";
    cout << "\n\t[4] Move Task to 'In Progress'";
    cout << "\n\t[5] Move Task to 'Done'";
    cout << "\n\t[6] Move Task to 'To Do'";
    cout << "\n\t[7] Delete Task";
    cout << "\n\t[8] Search Tasks";
    cout << "\n\t[9] Back to Main Menu";
    cout << "\n\n\tEnter your choice: ";
}

void KanbanBoardUI::displayAllTasks() const
{
    while (true)
    {
        KanbanUtils::clearScreen();
        KanbanUtils::displayHeader();

        if (board.isEmpty())
        {
            cout << "\n\tNo tasks available." << endl;
            cout << "\n\tPress Enter to return to menu...";
            cin.get();
            break;
        }

        vector<Task> todoTasks, inProgressTasks, doneTasks;
        board.getTasksByStatus(todoTasks, inProgressTasks, doneTasks);

        KanbanUtils::displayTasksInColumns(todoTasks, inProgressTasks, doneTasks);

        cout << "\n\n\tOptions:";
        cout << "\n\t[1] View task details";
        cout << "\n\t[2] Return to Kanban menu";
        cout << "\n\tEnter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\tInvalid input. Please enter a number. Press Enter to continue...";
            cin.get();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (choice == 1)
        {
            viewTaskDetails(); 
        }
        else if (choice == 2)
        {
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Press Enter to continue...";
            cin.get(); 
        }
    }
}

void KanbanBoardUI::viewTaskDetails() const
{
    cout << "\n\tEnter task ID to view details (0 to cancel): ";
    int id;
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\tInvalid input. Please enter a number. Press Enter to continue...";
        cin.get();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (id == 0)
    {
        cout << "\n\tOperation cancelled." << endl; 
        cout << "\n\tPress Enter to continue...";
        cin.get(); 
        return;
    }

    Task* task = board.findTaskById(id);
    if (task != nullptr)
    {
        KanbanUtils::clearScreen();
        KanbanUtils::displayHeader();
        cout << "\n\tTask Details:" << endl;
        cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;
        task->display();
        cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;
    }
    else
    {
        cout << "\n\tTask with ID " << id << " not found!" << endl;
    }

    cout << "\n\tPress Enter to continue...";
    cin.get(); 
}

void KanbanBoardUI::handleAddTask()
{
    KanbanUtils::clearScreen();
    KanbanUtils::displayHeader();

    string title, description, dueDate;

    cout << "\n\tEnter task title: ";
    getline(cin, title);
    while (title.empty()) {
        cout << "\tTitle cannot be empty. Enter task title: ";
        getline(cin, title);
    }

    cout << "\n\tEnter task description: ";
    getline(cin, description);
    while (description.empty()) {
        cout << "\tDescription cannot be empty. Enter task description: ";
        getline(cin, description);
    }

    do
    {
        cout << "\n\tEnter task due date (YYYY-MM-DD): ";
        getline(cin, dueDate);

        if (!KanbanUtils::isValidDate(dueDate))
        {
            cout << "\n\tInvalid date format or value. Please use YYYY-MM-DD format (e.g., 2025-05-04)." << endl;
        }
    } while (!KanbanUtils::isValidDate(dueDate));

    board.addTask(title, description, dueDate);

    cout << "\n\tTask added successfully!" << endl;
    cout << "\n\tPress Enter to continue...";
    cin.get(); 
}

void KanbanBoardUI::handleUpdateTask()
{
    KanbanUtils::clearScreen();
    KanbanUtils::displayHeader();

    int id;
    cout << "\n\tEnter task ID to update (0 to cancel): ";
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\tInvalid input. Please enter a number. Press Enter to continue...";
        cin.get();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (id == 0) {
        cout << "\n\tUpdate cancelled." << endl;
        cout << "\n\tPress Enter to continue...";
        cin.get();
        return;
    }

    Task* task = board.findTaskById(id);
    if (task != nullptr)
    {
        KanbanUtils::clearScreen();
        KanbanUtils::displayHeader();
        cout << "\n\tCurrent Task Details:" << endl;
        cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;
        task->display();
        cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;

        cout << "\n\tUpdate this task? (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (choice == 'y' || choice == 'Y')
        {
            string title, description, newDueDateStr;

            cout << "\n\tEnter new title (leave blank to keep current: '" << task->getTitle() << "'): ";
            getline(cin, title);

            cout << "\n\tEnter new description (leave blank to keep current: '" << task->getDescription() << "'): ";
            getline(cin, description);

            cout << "\n\tEnter new due date (YYYY-MM-DD) (leave blank to keep current: '" << task->getDueDate() << "'): ";
            getline(cin, newDueDateStr);

            string dueDate = ""; 
            if (!newDueDateStr.empty()) 
            {
                if (KanbanUtils::isValidDate(newDueDateStr)) 
                {
                    dueDate = newDueDateStr;
                }
                else {
                    cout << "\n\tInvalid date format entered. Due date will not be updated." << endl;
                }
            }

            cout << "\n\tUpdate status? (Current: " << task->getStatus() << ") (y/n): ";
            char statusChoiceChar;
            cin >> statusChoiceChar;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            string status = "";
            if (statusChoiceChar == 'y' || statusChoiceChar == 'Y')
            {
                cout << "\n\tSelect new status:";
                cout << "\n\t[1] To Do";
                cout << "\n\t[2] In Progress";
                cout << "\n\t[3] Done";
                cout << "\n\tEnter choice (1-3): ";

                int statusChoiceInt;
                cin >> statusChoiceInt;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "\n\tInvalid input. Status not updated.";
                }
                else
                {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    switch (statusChoiceInt)
                    {
                    case 1: status = "To Do"; break;
                    case 2: status = "In Progress"; break;
                    case 3: status = "Done"; break;
                    default: cout << "\n\tInvalid choice. Status not updated."; break;
                    }
                }
            }

            if (board.updateTask(id, title, description, dueDate, status)) {
                cout << "\n\tTask updated successfully!" << endl;
            }
            else {
                cout << "\n\tTask update failed (perhaps due to invalid data)." << endl;
            }
        }
        else
        {
            cout << "\n\tUpdate cancelled." << endl;
        }
    }
    else
    {
        cout << "\n\tTask with ID " << id << " not found!" << endl;
    }

    cout << "\n\tPress Enter to continue...";
    cin.get(); 
}

void KanbanBoardUI::handleDeleteTask()
{
    KanbanUtils::clearScreen();
    KanbanUtils::displayHeader();

    int id;
    cout << "\n\tEnter task ID to delete (0 to cancel): ";
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\tInvalid input. Please enter a number. Press Enter to continue...";
        cin.get();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (id == 0) {
        cout << "\n\tDeletion cancelled." << endl;
        cout << "\n\tPress Enter to continue...";
        cin.get();
        return;
    }

    Task* task = board.findTaskById(id);
    if (task != nullptr)
    {
        KanbanUtils::clearScreen();
        KanbanUtils::displayHeader();
        cout << "\n\tTask to be deleted:" << endl;
        cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;
        task->display();
        cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;

        cout << "\n\tAre you sure you want to delete this task? (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 'y' || choice == 'Y')
        {
            if (board.deleteTask(id))
            {
                cout << "\n\tTask deleted successfully!" << endl;
            }
            else 
            {
                cout << "\n\tError encountered during task deletion." << endl;
            }
        }
        else
        {
            cout << "\n\tDeletion cancelled." << endl;
        }
    }
    else
    {
        cout << "\n\tTask with ID " << id << " not found!" << endl;
    }

    cout << "\n\tPress Enter to continue...";
    cin.get(); 
}

void KanbanBoardUI::handleMoveTask(const string& newStatus)
{
    KanbanUtils::clearScreen();
    KanbanUtils::displayHeader();

    int id;
    cout << "\n\tEnter task ID to move to '" << newStatus << "' (0 to cancel): ";
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n\tInvalid input. Please enter a number.";
    }
    else
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        if (id == 0) {
            cout << "\n\tMove operation cancelled." << endl;
        }
        else {
            if (board.moveTask(id, newStatus))
            {
                cout << "\n\tTask moved to " << newStatus << " successfully!" << endl;
            }
            else
            {
                cout << "\n\tTask with ID " << id << " not found or move failed!" << endl;
            }
        }
    }

    cout << "\n\tPress Enter to continue...";
    cin.get(); 
}

void KanbanBoardUI::handleSearchTasks()
{
    KanbanUtils::clearScreen();
    KanbanUtils::displayHeader();

    string searchTerm;

    cout << "\n\tEnter search term (title or description, leave blank to cancel): ";
    getline(cin, searchTerm);

    if (searchTerm.empty()) {
        cout << "\n\tSearch cancelled." << endl;
        cout << "\n\tPress Enter to continue...";
        cin.get();
        return;
    }

    vector<Task> results = board.searchTasks(searchTerm);

    cout << "\n\tSearch Results for '" << searchTerm << "': " << results.size() << " found" << endl;
    cout << "\t" << KanbanUtils::horizontalLine('-', 80) << endl;

    if (results.empty())
    {
        cout << "\n\tNo tasks found matching your search." << endl;
    }
    else
    {
        cout << "\n\tTask IDs found: ";
        for (size_t i = 0; i < results.size(); ++i)
        {
            cout << results[i].getId() << (i == results.size() - 1 ? "" : ", ");
        }
        cout << endl;

        cout << "\n\tOptions:";
        cout << "\n\t[1] View a task's details";
        cout << "\n\t[2] Return to Kanban menu";
        cout << "\n\tEnter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\tInvalid input. Please enter a number. Returning to menu.";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            if (choice == 1) {
                viewTaskDetails(); 
            }
            else if (choice != 2) {
                cout << "\n\tInvalid choice. Returning to menu.";
            }
        }
    }

    cout << "\n\tPress Enter to continue...";
    cin.get(); 
}

void KanbanBoardUI::run()
{
    int choice;

    while (true)
    {
        displayMenu();
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\tInvalid input. Please enter a number. Press Enter to continue...";
            cin.get();
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            displayAllTasks(); 
            break;
        case 2:
            handleAddTask(); 
            break;
        case 3:
            handleUpdateTask(); 
            break;
        case 4:
            handleMoveTask("In Progress"); 
            break;
        case 5:
            handleMoveTask("Done"); 
            break;
        case 6:
            handleMoveTask("To Do"); 
            break;
        case 7:
            handleDeleteTask(); 
            break;
        case 8:
            handleSearchTasks(); 
            break;
        case 9:
            cout << "\n\tReturning to Main Menu..." << endl;
            KanbanUtils::displayFooter(); 
            return; 
        default:
            cout << "\n\tInvalid choice! Please try again." << endl;
            cout << "\n\tPress Enter to continue...";
            cin.get(); 
        }
    }
}