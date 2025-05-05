#ifndef CITATION_UTILS_H
#define CITATION_UTILS_H

#include <string>
#include <vector>
#include <limits>

using namespace std;

namespace CitationUtils 
{
    string getAppDataDirectory();
    string getCitationHistoryFilePath(const string& username); 

    bool createDirectoryIfNotExists(const string& path);

    bool saveCitationHistoryToFile(const vector<string>& citations, const string& username); 
    bool loadCitationHistoryFromFile(vector<string>& citations, const string& username); 
    void printSeparator();
    void clearConsoleScreen();
    void pauseConsole();
    string trimString(const string& str);

    void printCitationBuilderTitle();
}

#endif