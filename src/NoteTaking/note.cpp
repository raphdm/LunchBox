#include "../../include/NoteTaking/note.h"
#include "../../include/NoteTaking/notes_utils.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <stdexcept>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


using namespace std;

Note::Note()
{
    id = generateId();
    creationDate = getCurrentDateTime();
    lastModifiedDate = creationDate;
}

Note::Note(const string& title, const string& content)
    : title(title), content(content)
{
    id = generateId();
    creationDate = getCurrentDateTime();
    lastModifiedDate = creationDate;
}

Note::Note(const string& title, const string& content, const string& id,
    const string& creationDate, const string& lastModifiedDate)
    : title(title), content(content), id(id),
    creationDate(creationDate), lastModifiedDate(lastModifiedDate) {}

string Note::getTitle() const
{
    return title;
}

string Note::getContent() const
{
    return content;
}

string Note::getId() const
{
    return id;
}

string Note::getCreationDate() const
{
    return creationDate;
}

string Note::getLastModifiedDate() const
{
    return lastModifiedDate;
}

void Note::setTitle(const string& newTitle)
{
    title = newTitle;
    updateModificationDate();
}

void Note::setContent(const string& newContent)
{
    content = newContent;
    updateModificationDate();
}

void Note::updateModificationDate()
{
    lastModifiedDate = getCurrentDateTime();
}

string Note::toString() const
{
    stringstream ss;
    ss << "ID: " << id << "\n";
    ss << "Title: " << title << "\n";
    ss << "Created: " << creationDate << "\n";
    ss << "Modified: " << lastModifiedDate << "\n";
    ss << "Content:\n" << content << "\n";
    return ss.str();
}

string Note::getFilename() const
{
    string safeTitle = notes_utils::sanitizeFilename(title);

    if (safeTitle.empty() || safeTitle == "_")
    {
        return id + ".txt";
    }
    return safeTitle + "_" + id + ".txt";
}

string Note::generateId() const
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 15);

    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < 8; i++)
    {
        ss << setw(1) << dis(gen);
    }
    return ss.str();
}

string Note::getCurrentDateTime() const
{
    auto now = chrono::system_clock::now();
    auto timeT = chrono::system_clock::to_time_t(now);
    tm timeinfo;

    if (localtime_s(&timeinfo, &timeT) != 0) {
        throw runtime_error("Failed to get local time");
    }


    stringstream ss;
    ss << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw runtime_error("Failed to format time string");
    }
    return ss.str();
}