#include "../../include/CitationBuilder/citation_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <sys/stat.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <unistd.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

using namespace std;

string CitationUtils::getAppDataDirectory() 
{
    return "SavedData";
}

string CitationUtils::getCitationHistoryFilePath(const string& username) 
{
    return getAppDataDirectory() + "/CitationHistory/citation_history_" + username + ".txt"; 
}

bool CitationUtils::createDirectoryIfNotExists(const string& path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0) 
    {
        if (MKDIR(path.c_str()) != 0) 
        {
            cerr << "Error creating directory: " << path << endl;
            return false;
        }
    }
    else if (!(info.st_mode & S_IFDIR)) 
    {
        cerr << "Path exists but is not a directory: " << path << endl;
        return false;
    }

    return true;
}

bool CitationUtils::saveCitationHistoryToFile(const vector<string>& citations, const string& username) 
{ 
    string baseDirPath = getAppDataDirectory();
    string historyDirPath = getAppDataDirectory() + "/CitationHistory";
    string filePath = getCitationHistoryFilePath(username);

    if (!createDirectoryIfNotExists(baseDirPath))
    {
        return false;
    }
    if (!createDirectoryIfNotExists(historyDirPath)) 
    {
        return false;
    }

    ofstream file(filePath.c_str());
    if (!file.is_open()) {
        cerr << "Failed to open file for writing: " << filePath << endl;
        return false;
    }

    for (const auto& citation : citations) 
    {
        file << citation << "\n---\n";
    }

    file.close();
    return true;
}

bool CitationUtils::loadCitationHistoryFromFile(vector<string>& citations, const string& username) 
{ 
    string filePath = getCitationHistoryFilePath(username); 

    ifstream file(filePath.c_str());
    if (!file.is_open()) 
    {
        return false; 
    }

    citations.clear();
    string line, citation;

    while (getline(file, line)) 
    {
        if (line == "---") 
        {
            if (!citation.empty()) 
            {
                citations.push_back(citation);
                citation.clear();
            }
        }
        else {
            if (!citation.empty()) 
            {
                citation += '\n';
            }
            citation += line;
        }
    }

    if (!citation.empty()) 
    {
        citations.push_back(citation);
    }

    file.close();
    return true;
}

void CitationUtils::printSeparator() 
{
    cout << "===============================================" << endl;
}

void CitationUtils::clearConsoleScreen() 
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void CitationUtils::pauseConsole() 
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string CitationUtils::trimString(const string& str) 
{
    string result = str;

    size_t startPos = result.find_first_not_of(" \t\n\r\f\v");

    if (startPos == string::npos) 
    {
        return "";
    }

    size_t endPos = result.find_last_not_of(" \t\n\r\f\v");

    return result.substr(startPos, endPos - startPos + 1);
}

void CitationUtils::printCitationBuilderTitle() 
{
    CitationUtils::printSeparator();
    cout << "            CITATION BUILDER \n";
    CitationUtils::printSeparator();
}