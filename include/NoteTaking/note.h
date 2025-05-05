#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <memory>

using namespace std;

namespace notes_utils {
    string sanitizeFilename(const string& filename);
}

class Note
{
private:
    string title;
    string content;
    string id;
    string creationDate;
    string lastModifiedDate;

public:
    Note();
    Note(const string& title, const string& content);
    Note(const string& title, const string& content, const string& id,
        const string& creationDate, const string& lastModifiedDate);

    string getTitle() const;
    string getContent() const;
    string getId() const;
    string getCreationDate() const;
    string getLastModifiedDate() const;

    void setTitle(const string& newTitle);
    void setContent(const string& newContent);
    void updateModificationDate();

    string toString() const;
    string getFilename() const;

private:
    string generateId() const;
    string getCurrentDateTime() const;
};

#endif