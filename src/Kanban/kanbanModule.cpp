#include "../../include/Kanban/kanbanModule.h"
#include "../../include/Kanban/kanbanBoard.h"
#include "../../include/Kanban/kanbanBoardUI.h"
#include "../../include/auth.h" 
#include <iostream>
#include <cstdlib> 
#include <string> 

using namespace std;

string KanbanModule::getName() const
{
    return "Kanban Board";
}

void KanbanModule::run(User& user)
{
    string currentUsername = user.getUsername();

    if (currentUsername.empty()) {
        cerr << "Error: Cannot run Kanban module without a logged-in user." << endl;
        cout << "Please log out and log back in." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    KanbanBoard board(currentUsername);
    KanbanBoardUI ui(board);
    ui.run();
}