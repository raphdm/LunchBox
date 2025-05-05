#ifndef NOTES_UTILS_H
#define NOTES_UTILS_H

#include <string>
#include <vector>
#include <limits>
#include <iostream>

using namespace std;

namespace notes_utils
{
    string trim(const string& str);
    vector<string> split(const string& str, char delimiter);
    string join(const vector<string>& strings, const string& delimiter);

    string toLower(const string& str);
    bool containsIgnoreCase(const string& haystack, const string& needle);

    bool isInteger(const string& str);
    bool stringToInt(const string& str, int& outValue);

    string replaceAll(const string& str, const string& from, const string& to);
    string sanitizeFilename(const string& filename);

    string readMultilineInput();

    string toUpper(const string& str);

    void waitForEnter();
}

#endif