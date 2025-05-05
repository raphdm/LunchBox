#include "../../include/NoteTaking/noteManager.h"
#include "../../include/NoteTaking/notes_utils.h"
#include <algorithm>
#include <iostream>

using namespace std;

NoteManager::NoteManager(const string& userDirectory, shared_ptr<FileHandler> fh)
    : dataDirectory(userDirectory), fileHandler(fh)
{
    if (fileHandler)
    {
        loadNotes();
    }
    else 
    {
        cerr << "Error: FileHandler not provided to NoteManager." << endl;
    }
}

void NoteManager::loadNotes()
{
    if (!ensureDataDirectoryExists())
    {
        cerr << "Warning: Could not ensure data directory exists: " << dataDirectory << endl;
        return;
    }

    notes.clear();
    vector<string> files = fileHandler->listFiles(dataDirectory);

    for (const auto& file : files)
    {
        if (file.length() > 4 && file.substr(file.length() - 4) == ".txt")
        {
            string filePath = getNotePath(file);
            shared_ptr<Note> note = fileHandler->loadNoteFromFile(filePath);

            if (note)
            {
                notes.push_back(note);
            }
            else {
                cerr << "Warning: Failed to load note from file: " << filePath << endl;
            }
        }
    }
    cout << "Loaded " << notes.size() << " notes for the user." << endl;
}

bool NoteManager::saveNote(const shared_ptr<Note>& note)
{
    if (!note || !fileHandler)
    {
        cerr << "Error: Cannot save note. Null note or file handler." << endl;
        return false;
    }

    note->updateModificationDate();

    bool found = false;
    for (size_t i = 0; i < notes.size(); i++)
    {
        if (notes[i]->getId() == note->getId())
        {
            notes[i] = note;
            found = true;
            break;
        }
    }

    if (!found)
    {
        notes.push_back(note);
    }

    if (!fileHandler->saveNoteToFile(note, dataDirectory))
    {
        cerr << "Error: Failed to save note to file system: " << getNotePath(note) << endl;
        if (!found)
        {
            notes.pop_back();
        }
        return false;
    }
    return true;
}


bool NoteManager::deleteNote(const string& id)
{
    if (!fileHandler)
    {
        cerr << "Error: Cannot delete note. File handler not available." << endl;
        return false;
    }

    for (auto it = notes.begin(); it != notes.end(); ++it)
    {
        if ((*it)->getId() == id)
        {
            string filePath = getNotePath(*it);
            bool deleted = fileHandler->deleteFile(filePath);

            if (deleted)
            {
                notes.erase(it);
                return true;
            }
            else
            {
                cerr << "Error: Failed to delete note file: " << filePath << endl;
                return false;
            }
        }
    }

    cerr << "Warning: Note with ID " << id << " not found for deletion." << endl;
    return false;
}

shared_ptr<Note> NoteManager::createNote(const string& title, const string& content)
{
    shared_ptr<Note> note = make_shared<Note>(title, content);

    if (saveNote(note))
    {
        return note;
    }

    cerr << "Error: Failed to save newly created note." << endl;
    return nullptr;
}

shared_ptr<Note> NoteManager::getNoteById(const string& id)
{
    for (const auto& note : notes)
    {
        if (note->getId() == id)
        {
            return note;
        }
    }

    return nullptr;
}

shared_ptr<Note> NoteManager::getNoteByTitle(const string& title)
{
    string titleLower = notes_utils::toLower(title);

    for (const auto& note : notes)
    {
        if (notes_utils::toLower(note->getTitle()) == titleLower)
        {
            return note;
        }
    }

    return nullptr;
}

vector<shared_ptr<Note>> NoteManager::getAllNotes() const
{
    return notes;
}

vector<shared_ptr<Note>> NoteManager::searchNotes(const string& searchTerm) const
{
    vector<shared_ptr<Note>> results;
    if (searchTerm.empty()) {
        return results;
    }
    string termLower = notes_utils::toLower(searchTerm);

    for (const auto& note : notes)
    {
        if (notes_utils::containsIgnoreCase(note->getTitle(), termLower) ||
            notes_utils::containsIgnoreCase(note->getContent(), termLower))
        {
            results.push_back(note);
        }
    }

    return results;
}

int NoteManager::getNotesCount() const
{
    return static_cast<int>(notes.size());
}

bool NoteManager::isDataDirectoryAccessible() const
{
    return fileHandler && fileHandler->directoryExists(dataDirectory);
}

bool NoteManager::ensureDataDirectoryExists()
{
    if (!fileHandler) return false;
    if (!fileHandler->directoryExists(dataDirectory))
    {
        return fileHandler->createDirectory(dataDirectory);
    }

    return true;
}

string NoteManager::getNotePath(const shared_ptr<Note>& note) const
{
    if (!note) return "";
    return dataDirectory + "\\" + note->getFilename();
}

string NoteManager::getNotePath(const string& filename) const
{
    return dataDirectory + "\\" + filename;
}