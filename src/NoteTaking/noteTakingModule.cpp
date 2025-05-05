#include "../../include/NoteTaking/noteTakingModule.h"
#include "../../include/NoteTaking/fileHandler.h"
#include "../../include/NoteTaking/noteManager.h"
#include "../../include/NoteTaking/notes_ui.h"
#include "../../include/utils.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

string NoteTakingModule::getName() const
{
    return "Note Taking";
}

void NoteTakingModule::run(User& user)
{
    string currentUsername = user.getUsername();
    if (currentUsername.empty()) 
    {
        cerr << "Error: Cannot run Note Taking module without a logged-in user." << endl;
        cout << "Please log out and log back in." << endl;
        pauseScreen();
        return;
    }

    try 
    {
        string userNotesDir = "SavedData\\Notes\\" + currentUsername;

        shared_ptr<FileHandler> fileHandler = make_shared<FileHandler>();

        shared_ptr<NoteManager> noteManager = make_shared<NoteManager>(userNotesDir, fileHandler);


        NoteTakingUI ui(noteManager);
        ui.runUserInterfaceLoop();

    }
    catch (const invalid_argument& e) 
    {
        cerr << "Initialization Error: " << e.what() << endl;
        pauseScreen();
    }
    catch (const exception& e) 
    {
        cerr << "An unexpected error occurred: " << e.what() << endl;
        pauseScreen();
    }
    catch (...) 
    {
        cerr << "An unknown error occurred during Note Taking module execution." << endl;
        pauseScreen();
    }
}