#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include "note.h"
#include "fileHandler.h"

using namespace std;

class NoteManager
{
private:
    vector<shared_ptr<Note>> notes;
    string dataDirectory;
    shared_ptr<FileHandler> fileHandler;

public:
    NoteManager(const string& userDirectory, shared_ptr<FileHandler> fh);

    void loadNotes();
    bool saveNote(const shared_ptr<Note>& note);
    bool deleteNote(const string& id);

    shared_ptr<Note> createNote(const string& title, const string& content);
    shared_ptr<Note> getNoteById(const string& id);
    shared_ptr<Note> getNoteByTitle(const string& title);

    vector<shared_ptr<Note>> getAllNotes() const;
    vector<shared_ptr<Note>> searchNotes(const string& searchTerm) const;

    int getNotesCount() const;
    bool isDataDirectoryAccessible() const;

private:
    bool ensureDataDirectoryExists();
    string getNotePath(const shared_ptr<Note>& note) const;
    string getNotePath(const string& filename) const;
};

#endif