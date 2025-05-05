#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <memory>
#include "note.h"
#include <direct.h>
#include <cstdio>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

class FileHandler
{
public:
    bool createDirectory(const string& path);
    bool directoryExists(const string& path);
    vector<string> listFiles(const string& directoryPath);

    bool writeToFile(const string& filePath, const string& content);
    string readFromFile(const string& filePath);
    bool deleteFile(const string& filePath);

    bool saveNoteToFile(const shared_ptr<Note>& note, const string& directory);
    shared_ptr<Note> loadNoteFromFile(const string& filePath);
};

#endif