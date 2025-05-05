#include "../../include/NoteTaking/notes_ui.h"
#include "../../include/NoteTaking/notes_utils.h"
#include "../../include/utils.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <stdexcept>
#include <algorithm>

using namespace std;

NoteTakingUI::NoteTakingUI(shared_ptr<NoteManager> manager)
    : noteManager(manager), running(false)
{
    if (!noteManager) 
    {
        throw invalid_argument("NoteManager cannot be null.");
    }
}

void NoteTakingUI::runUserInterfaceLoop()
{
    if (!noteManager->isDataDirectoryAccessible()) 
    {
        cerr << "Error: Cannot access or create the notes data directory." << endl;
        cerr << "Note Taking feature may not function correctly." << endl;
        pauseScreen();
    }

    running = true;
    int choice;

    vector<string> options = 
    {
        "Create new note",
        "List all notes",
        "View note",
        "Edit note",
        "Delete note",
        "Search notes",
        "Back to Main Menu"
    };
    const int MAX_CHOICE = static_cast<int>(options.size());
    const int EXIT_CHOICE = MAX_CHOICE;


    while (running)
    {
        displayMainMenu();
        cout << "===================================" << endl;
        cout << "Total notes: " << noteManager->getNotesCount() << endl;
        cout << "===================================" << endl;
        choice = getIntInput("Enter your choice (1-" + to_string(MAX_CHOICE) + "): ", 1, MAX_CHOICE);

        if (choice == -1) { // getIntInput returns min-1 on failure/max attempts
            cout << "Invalid input after multiple attempts. Returning to menu." << endl;
            pauseScreen();
            continue;
        }

        handleUserChoice(choice);

    }
}


void NoteTakingUI::displayMainMenu()
{
    displayHeader("NOTE TAKING");
    vector<string> options = 
    {
        "Create new note",
        "List all notes",
        "View note",
        "Edit note",
        "Delete note",
        "Search notes",
        "Back to Main Menu"
    };

    displayMenuOptions(options);
}

void NoteTakingUI::handleUserChoice(int choice)
{
    const int EXIT_CHOICE = 7; 

    switch (choice)
    {
    case 1:
        createNewNote();
        break;
    case 2:
        listAllNotes();
        break;
    case 3:
        viewNote();
        break;
    case 4:
        editNote();
        break;
    case 5:
        deleteNote();
        break;
    case 6:
        searchNotes();
        break;
    case EXIT_CHOICE: 
        running = false;
        cout << "\nReturning to main application menu..." << endl;
        Sleep(600);
        break;
    default:
        cout << "Invalid choice. Please try again.\n";
        pauseScreen();
        break;
    }
}

void NoteTakingUI::createNewNote()
{
    displayHeader("CREATE NEW NOTE");

    string title = getStringInput("Enter note title: ", false, 1, 100);
    if (title.empty() && cin.fail())
    {
        cout << "Note creation cancelled.\n";
        pauseScreen();
        return;
    }
    if (title.empty())
    {
        cout << "Note creation cancelled. Title cannot be empty.\n";
        pauseScreen();
        return;
    }


    string content = notes_utils::readMultilineInput();

    shared_ptr<Note> newNote = noteManager->createNote(title, content);
    if (newNote)
    {
        cout << "\nNote created successfully!\n";
        displayNote(newNote);
    }
    else
    {
        cout << "\nFailed to create note. Please check file permissions or disk space.\n";
    }

    notes_utils::waitForEnter();
}

void NoteTakingUI::listAllNotes()
{
    displayHeader("ALL NOTES");

    vector<shared_ptr<Note>> allNotes = noteManager->getAllNotes();
    if (allNotes.empty())
    {
        cout << "No notes found.\n";
        pauseScreen();
        return;
    }


    cout << "--- Default Order --- \n";
    cout << left << setw(5) << "#" << setw(10) << "ID" << setw(30) << "Title" << setw(20) << "Created" << "Modified\n";
    cout << string(85, '-') << "\n";
    for (size_t i = 0; i < allNotes.size(); ++i)
    {
        const auto& note = allNotes[i];
        string numLabel = "[" + to_string(i + 1) + "]";
        cout << left << setw(5) << numLabel
            << setw(10) << note->getId()
            << setw(30) << (note->getTitle().length() > 28 ? note->getTitle().substr(0, 27) + "..." : note->getTitle())
            << setw(20) << note->getCreationDate()
            << note->getLastModifiedDate() << "\n";
    }
    cout << "-------------------------------------------------------------------------------------\n\n";

    vector<string> sortOptions = 
    {
         "Alphabetical (A-Z)",
         "By Date Created (Newest First)",
         "By Date Modified (Newest First)",
         "Alphabetical (Z-A)",
         "By Date Created (Oldest First)",
         "By Date Modified (Oldest First)",
         "Keep Default Order"
    };
    cout << "How would you like to sort the notes?\n";
    displayMenuOptions(sortOptions);


    int sortChoice = getIntInput("Enter sort choice: ", 1, static_cast<int>(sortOptions.size()));


    string sortMessage = "\nDisplaying notes in default order.\n";
    bool sortApplied = false;

    if (sortChoice != -1 && sortChoice != static_cast<int>(sortOptions.size())) 
    {
        sortApplied = true;
        switch (sortChoice)
        {
        case 1:
            sort(allNotes.begin(), allNotes.end(),
                [](const shared_ptr<Note>& a, const shared_ptr<Note>& b)
                {
                    return notes_utils::toLower(a->getTitle()) < notes_utils::toLower(b->getTitle());
                });
            sortMessage = "\nSorted Alphabetically (A-Z).\n";
            break;
        case 2:
            sort(allNotes.begin(), allNotes.end(),
                [](const shared_ptr<Note>& a, const shared_ptr<Note>& b)
                {
                    return a->getCreationDate() > b->getCreationDate();
                });
            sortMessage = "\nSorted by Creation Date (Newest First).\n";
            break;
        case 3:
            sort(allNotes.begin(), allNotes.end(),
                [](const shared_ptr<Note>& a, const shared_ptr<Note>& b)
                {
                    return a->getLastModifiedDate() > b->getLastModifiedDate();
                });
            sortMessage = "\nSorted by Modification Date (Newest First).\n";
            break;
        case 4:
            sort(allNotes.begin(), allNotes.end(),
                [](const shared_ptr<Note>& a, const shared_ptr<Note>& b)
                {
                    return notes_utils::toLower(a->getTitle()) > notes_utils::toLower(b->getTitle());
                });
            sortMessage = "\nSorted Alphabetically (Z-A).\n";
            break;
        case 5:
            sort(allNotes.begin(), allNotes.end(),
                [](const shared_ptr<Note>& a, const shared_ptr<Note>& b)
                {
                    return a->getCreationDate() < b->getCreationDate();
                });
            sortMessage = "\nSorted by Creation Date (Oldest First).\n";
            break;
        case 6:
            sort(allNotes.begin(), allNotes.end(),
                [](const shared_ptr<Note>& a, const shared_ptr<Note>& b)
                {
                    return a->getLastModifiedDate() < b->getLastModifiedDate();
                });
            sortMessage = "\nSorted by Modification Date (Oldest First).\n";
            break;
        default:
            sortApplied = false;
            sortMessage = "\nInvalid sort choice. Displaying notes in default order.\n";
            break;
        }
    }
    else if (sortChoice == -1) 
    {
        sortApplied = false;
        sortMessage = "\nSort cancelled. Displaying notes in default order.\n";
    }


    if (sortApplied) 
    {
        displayHeader("ALL NOTES");
        cout << sortMessage;
        cout << "-------------------------------------------------------------------------------------\n";

        cout << left << setw(5) << "#" << setw(10) << "ID" << setw(30) << "Title" << setw(20) << "Created" << "Modified\n";
        cout << string(85, '-') << "\n";

        for (size_t i = 0; i < allNotes.size(); ++i)
        {
            const auto& note = allNotes[i];
            string numLabel = "[" + to_string(i + 1) + "]";
            cout << left << setw(5) << numLabel
                << setw(10) << note->getId()
                << setw(30) << (note->getTitle().length() > 28 ? note->getTitle().substr(0, 27) + "..." : note->getTitle())
                << setw(20) << note->getCreationDate()
                << note->getLastModifiedDate() << "\n";
        }
        cout << string(85, '-') << "\n";
    }
    else {

    }


    cout << "\nTotal notes: " << allNotes.size() << "\n";
    notes_utils::waitForEnter();
}

void NoteTakingUI::viewNote()
{
    shared_ptr<Note> note = selectNote("view");
    if (note)
    {
        displayHeader("NOTE DETAILS");
        displayNote(note);
        notes_utils::waitForEnter();
    }
}

void NoteTakingUI::editNote()
{
    shared_ptr<Note> note = selectNote("edit");
    if (!note)
    {
        return;
    }

    displayHeader("EDIT NOTE (" + note->getId() + ")");
    displayNote(note);

    vector<string> editOptions = {
        "Title",
        "Content (Overwrite)",
        "Both (Overwrite Content)",
        "Content (Append)",
        "Cancel"
    };
    cout << "\nWhat would you like to edit?\n";
    displayMenuOptions(editOptions);

    int choice = getIntInput("Enter your choice: ", 1, static_cast<int>(editOptions.size()));

    if (choice == -1 || choice == static_cast<int>(editOptions.size())) 
    {
        cout << "Edit cancelled.\n";
        pauseScreen();
        return;
    }

    string newTitle = note->getTitle();
    string newContent = note->getContent();
    bool titleChanged = false;
    bool contentChanged = false;

    if (choice == 1 || choice == 3)
    {
        newTitle = getStringInput("Enter new title: ", false, 1, 100);
        if (newTitle.empty() && cin.fail()) {
            cout << "Edit cancelled due to invalid input." << endl;
            pauseScreen();
            return;
        }
        if (newTitle.empty())
        {
            cout << "Title cannot be empty. Using original title.\n";
            newTitle = note->getTitle();
        }
        else 
        {
            titleChanged = (newTitle != note->getTitle());
        }
    }

    if (choice == 2 || choice == 3)
    {
        cout << "\nEnter new content below.\n";
        newContent = notes_utils::readMultilineInput();
        contentChanged = (newContent != note->getContent());
    }
    else if (choice == 4)
    {
        cout << "\nEnter content to append below.\n";
        string appendContent = notes_utils::readMultilineInput();
        if (!appendContent.empty())
        {
            newContent += (newContent.empty() ? "" : "\n") + appendContent;
            contentChanged = true;
        }
        else
        {
            cout << "No content entered to append.\n";
        }
    }

    if (titleChanged)
    {
        note->setTitle(newTitle);
    }
    if (contentChanged)
    {
        note->setContent(newContent);
    }


    if (titleChanged || contentChanged)
    {
        if (!titleChanged && contentChanged) 
        {
            note->updateModificationDate();
        }

        if (noteManager->saveNote(note))
        {
            cout << "\nNote updated successfully!\n";
            displayHeader("UPDATED NOTE (" + note->getId() + ")");
            displayNote(note);
        }
        else
        {
            cout << "\nFailed to update note. Please try again.\n";
        }
    }
    else
    {
        cout << "\nNo changes were made to the note.\n";
    }

    notes_utils::waitForEnter();
}


void NoteTakingUI::deleteNote()
{
    shared_ptr<Note> note = selectNote("delete");
    if (!note) 
    {
        return;
    }

    displayHeader("DELETE NOTE");
    displayNote(note);

    if (getYesNoInput("\nAre you sure you want to delete this note?"))
    {
        if (noteManager->deleteNote(note->getId()))
        {
            cout << "Note deleted successfully!\n";
        }
        else
        {
            cout << "Failed to delete note. Please try again.\n";
        }
    }
    else {
        cout << "Deletion cancelled.\n";
    }

    pauseScreen();
}

void NoteTakingUI::searchNotes()
{
    displayHeader("SEARCH NOTES");

    string searchTerm = getStringInput("Enter search term: ", true, 0, 100);
    if (searchTerm.empty() && cin.fail()) 
    {
        cout << "Search cancelled.\n";
        pauseScreen();
        return;
    }
    if (searchTerm.empty())
    {
        cout << "Search term cannot be empty.\n";
        pauseScreen();
        return;
    }

    vector<shared_ptr<Note>> results = noteManager->searchNotes(searchTerm);

    cout << "\nSearch results for \"" << searchTerm << "\":\n\n";

    if (results.empty()) 
    {
        cout << "No notes found matching the search term.\n";
        pauseScreen();
        return;
    }

    cout << left << setw(5) << "#" << setw(10) << "ID" << setw(30) << "Title" << setw(20) << "Created" << "Modified\n";
    cout << string(85, '-') << "\n";

    for (size_t i = 0; i < results.size(); ++i) 
    {
        const auto& note = results[i];
        string numLabel = "[" + to_string(i + 1) + "]";
        cout << left << setw(5) << numLabel
            << setw(10) << note->getId()
            << setw(30) << (note->getTitle().length() > 28 ? note->getTitle().substr(0, 27) + "..." : note->getTitle())
            << setw(20) << note->getCreationDate()
            << note->getLastModifiedDate() << "\n";
    }
    cout << string(85, '-') << "\n";

    cout << "\nFound " << results.size() << " notes matching the search term.\n";
    notes_utils::waitForEnter();
}

void NoteTakingUI::displayNote(const shared_ptr<Note>& note)
{
    if (!note) 
    {
        cout << "No note to display.\n";
        return;
    }

    cout << "\n----------------------------------------\n";
    cout << "ID: " << note->getId() << "\n";
    cout << "Title: " << note->getTitle() << "\n";
    cout << "Created: " << note->getCreationDate() << "\n";
    cout << "Modified: " << note->getLastModifiedDate() << "\n";
    cout << "----------------------------------------\n";
    cout << "Content:\n\n" << note->getContent() << "\n";
    cout << "----------------------------------------\n";
}

shared_ptr<Note> NoteTakingUI::selectNote(const string& action)
{
    vector<shared_ptr<Note>> allNotes = noteManager->getAllNotes();

    if (allNotes.empty())
    {
        displayHeader("SELECT NOTE TO " + notes_utils::toUpper(action));
        cout << "No notes available to " << action << ".\n";
        pauseScreen();
        return nullptr;
    }

    while (true)
    {
        displayHeader("SELECT NOTE TO " + notes_utils::toUpper(action));

        cout << left << setw(5) << "#" << setw(10) << "ID" << setw(30) << "Title" << setw(20) << "Created" << "Modified\n";
        cout << string(85, '-') << "\n";
        for (size_t i = 0; i < allNotes.size(); ++i)
        {
            const auto& note = allNotes[i];
            string numLabel = "[" + to_string(i + 1) + "]";
            cout << left << setw(5) << numLabel
                << setw(10) << note->getId()
                << setw(30) << (note->getTitle().length() > 28 ? note->getTitle().substr(0, 27) + "..." : note->getTitle())
                << setw(20) << note->getCreationDate()
                << note->getLastModifiedDate() << "\n";
        }
        cout << string(85, '-') << "\n";
        cout << "Enter list number [#], note ID, or note Title to select.\n";
        cout << "Enter '0' or leave blank to cancel.\n";

        string input = getStringInput("\nYour selection: ", true, 0, 100);

        if (input.empty() && cin.fail()) 
        {
            cout << "Operation cancelled due to invalid input." << endl;
            pauseScreen();
            return nullptr;
        }

        if (input.empty() || input == "0")
        {
            cout << "Operation cancelled.\n";
            pauseScreen();
            return nullptr;
        }

        int noteNum = 0;
        if (notes_utils::stringToInt(input, noteNum))
        {
            if (noteNum >= 1 && noteNum <= static_cast<int>(allNotes.size()))
            {
                return allNotes[noteNum - 1];
            }
            else
            {
                cerr << "Error: Invalid list number: " << noteNum << ". Please enter a number between 1 and " << allNotes.size() << ".\n";
                pauseScreen();
                continue;
            }
        }


        shared_ptr<Note> noteById = noteManager->getNoteById(input);
        if (noteById)
        {
            return noteById;
        }


        shared_ptr<Note> noteByTitle = noteManager->getNoteByTitle(input);
        if (noteByTitle)
        {
            return noteByTitle;
        }

        cerr << "Error: Invalid input. No note found matching \"" << input << "\" as a list number, ID, or Title.\n";
        pauseScreen();
    }
}