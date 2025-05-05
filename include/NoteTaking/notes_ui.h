#ifndef NOTES_UI_H
#define NOTES_UI_H

#include <string>
#include <memory>
#include <vector>
#include "noteManager.h"
#include "note.h"

using namespace std;

class NoteTakingUI
{
private:
    shared_ptr<NoteManager> noteManager;
    bool running;

public:
    NoteTakingUI(shared_ptr<NoteManager> manager);
    void runUserInterfaceLoop();

private:
    void displayMainMenu();
    void handleUserChoice(int choice);

    void createNewNote();
    void listAllNotes();
    void viewNote();
    void editNote();
    void deleteNote();
    void searchNotes();

    void displayNote(const shared_ptr<Note>& note);
    shared_ptr<Note> selectNote(const string& action);

};

#endif