#include <fstream>
#include <sstream>
#include <cerrno>
#include <windows.h>
#include <direct.h>
#include <cstdio>
#include <iostream>
#include <cstring> 
#include "../../include/NoteTaking/fileHandler.h"

using namespace std;

string getErrorMessage(int errnum) 
{
    char buffer[256]; 
    if (strerror_s(buffer, sizeof(buffer), errnum) == 0) {
        return string(buffer);
    }
    else {
        return "Unknown error (" + to_string(errnum) + ")";
    }
}


bool FileHandler::createDirectory(const string& path)
{
    string currentPath = "";
    stringstream ss(path);
    string folder;
    vector<string> folders;

    size_t pos = 0;
    string delimiter = "\\";
    string token;
    string tempPath = path;

    if (tempPath.back() == '\\')
    {
        tempPath.pop_back();
    }

    while ((pos = tempPath.find(delimiter)) != string::npos)
    {
        token = tempPath.substr(0, pos);
        if (!token.empty())
        {
            folders.push_back(token);
        }
        tempPath.erase(0, pos + delimiter.length());
    }
    if (!tempPath.empty())
    {
        folders.push_back(tempPath);
    }


    for (size_t i = 0; i < folders.size(); ++i)
    {
        if (i == 0)
        {
            currentPath = folders[i];
        }
        else
        {
            currentPath += "\\" + folders[i];
        }


        DWORD fileAttr = GetFileAttributesA(currentPath.c_str());
        if (fileAttr == INVALID_FILE_ATTRIBUTES)
        {
            if (_mkdir(currentPath.c_str()) != 0 && errno != EEXIST)
            {
                cerr << "Failed to create directory: " << currentPath << " Error: " << getErrorMessage(errno) << endl;
                return false;
            }
        }
        else if (!(fileAttr & FILE_ATTRIBUTE_DIRECTORY))
        {
            cerr << "Path exists but is not a directory: " << currentPath << endl;
            return false;
        }
    }

    return true;
}

bool FileHandler::directoryExists(const string& path)
{
    DWORD fileAttr = GetFileAttributesA(path.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    return (fileAttr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

vector<string> FileHandler::listFiles(const string& directoryPath)
{
    vector<string> files;
    string searchPath = directoryPath + "\\*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do {
            string filename = findData.cFileName;
            if (filename != "." && filename != "..")
            {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    files.push_back(filename);
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
    else {

    }
    return files;
}

bool FileHandler::writeToFile(const string& filePath, const string& content)
{
    ofstream file(filePath, ios::binary);
    if (!file.is_open())
    {
        cerr << "Failed to open file for writing: " << filePath << endl;
        return false;
    }

    file.write(content.c_str(), content.length());
    bool success = file.good();
    file.close();
    if (!success) {
        cerr << "Failed to write content to file: " << filePath << endl;
    }
    return success;
}

string FileHandler::readFromFile(const string& filePath)
{
    ifstream file(filePath, ios::binary);
    if (!file.is_open())
    {
        cerr << "Failed to open file for reading: " << filePath << endl;
        return "";
    }

    ostringstream buffer;
    buffer << file.rdbuf();
    if (file.bad()) {
        cerr << "Error reading from file: " << filePath << endl;
    }
    file.close();
    return buffer.str();
}

bool FileHandler::deleteFile(const string& filePath)
{
    if (remove(filePath.c_str()) != 0) {
        if (errno != ENOENT) {
            cerr << "Error deleting file: " << filePath << " - " << getErrorMessage(errno) << endl;
        }
        return false;
    }
    return true;
}

bool FileHandler::saveNoteToFile(const shared_ptr<Note>& note, const string& directory)
{
    if (!note)
    {
        cerr << "Cannot save null note." << endl;
        return false;
    }

    if (!directoryExists(directory))
    {
        if (!createDirectory(directory))
        {
            cerr << "Failed to create directory for saving note: " << directory << endl;
            return false;
        }
    }

    string filePath = directory + "\\" + note->getFilename();

    stringstream contentStream;
    contentStream << "TITLE: " << note->getTitle() << "\n";
    contentStream << "ID: " << note->getId() << "\n";
    contentStream << "CREATED: " << note->getCreationDate() << "\n";
    contentStream << "MODIFIED: " << note->getLastModifiedDate() << "\n";
    contentStream << "CONTENT:" << "\n";
    contentStream << note->getContent();

    return writeToFile(filePath, contentStream.str());
}

shared_ptr<Note> FileHandler::loadNoteFromFile(const string& filePath)
{
    string fileContent = readFromFile(filePath);
    if (fileContent.empty())
    {

        return nullptr;
    }

    string title, id, creationDate, lastModifiedDate;
    stringstream noteContentBuffer;
    bool contentSectionStarted = false;

    stringstream contentStream(fileContent);
    string line;

    while (getline(contentStream, line))
    {

        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (!contentSectionStarted && line == "CONTENT:")
        {
            contentSectionStarted = true;
            continue;
        }

        if (contentSectionStarted)
        {
            noteContentBuffer << line << '\n';
        }
        else
        {
            if (line.rfind("TITLE: ", 0) == 0)
            {
                title = line.substr(7);
            }
            else if (line.rfind("ID: ", 0) == 0)
            {
                id = line.substr(4);
            }
            else if (line.rfind("CREATED: ", 0) == 0)
            {
                creationDate = line.substr(9);
            }
            else if (line.rfind("MODIFIED: ", 0) == 0)
            {
                lastModifiedDate = line.substr(10);
            }
        }
    }

    string noteContentText = noteContentBuffer.str();
    if (!noteContentText.empty() && noteContentText.back() == '\n')
    {
        noteContentText.pop_back();
    }

    if (id.empty())
    {
        cerr << "Failed to load note: ID is missing in file " << filePath << endl;
        return nullptr;
    }

    return make_shared<Note>(title, noteContentText, id, creationDate, lastModifiedDate);
}