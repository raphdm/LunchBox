#include "../../include/NoteTaking/notes_utils.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <limits>
#include <stdexcept>

using namespace std;

namespace notes_utils
{

    string trim(const string& str)
    {
        size_t first = str.find_first_not_of(" \t\n\r\f\v");

        if (first == string::npos)
        {
            return "";
        }

        size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, (last - first + 1));
    }

    vector<string> split(const string& str, char delimiter)
    {
        vector<string> tokens;
        string token;
        istringstream tokenStream(str);

        while (getline(tokenStream, token, delimiter))
        {
            string trimmedToken = trim(token);
            if (!trimmedToken.empty())
            {
                tokens.push_back(trimmedToken);
            }
        }

        return tokens;
    }

    string join(const vector<string>& strings, const string& delimiter)
    {
        if (strings.empty())
        {
            return "";
        }

        stringstream result;
        result << strings[0];

        for (size_t i = 1; i < strings.size(); ++i)
        {
            result << delimiter << strings[i];
        }

        return result.str();
    }

    bool containsIgnoreCase(const string& haystack, const string& needle)
    {
        if (needle.empty())
        {
            return true;
        }
        if (haystack.empty())
        {
            return false;
        }

        string haystackLower = toLower(haystack);
        string needleLower = toLower(needle);

        return haystackLower.find(needleLower) != string::npos;
    }

    bool isInteger(const string& str)
    {
        string trimmedStr = trim(str);
        if (trimmedStr.empty())
        {
            return false;
        }

        size_t start = 0;
        if (trimmedStr[0] == '+' || trimmedStr[0] == '-')
        {
            start = 1;
            if (trimmedStr.length() == 1) return false;
        }

        for (size_t i = start; i < trimmedStr.length(); ++i)
        {
            if (!isdigit(trimmedStr[i])) 
            {
                return false;
            }
        }

        return true;
    }

    bool stringToInt(const string& str, int& outValue)
    {
        string trimmedStr = trim(str);
        if (!isInteger(trimmedStr))
        {
            return false;
        }

        try
        {
            size_t processedChars = 0;
            long long tempValue = stoll(trimmedStr, &processedChars);

            if (processedChars != trimmedStr.length())
            {
                return false;
            }

            if (tempValue < numeric_limits<int>::min() || tempValue > numeric_limits<int>::max()) 
            {
                return false;
            }

            outValue = static_cast<int>(tempValue);
            return true;
        }
        catch (const out_of_range&)
        {
            return false;
        }
        catch (const invalid_argument&)
        {
            return false;
        }
        catch (...)
        {
            return false;
        }
    }

    string replaceAll(const string& str, const string& from, const string& to)
    {
        if (from.empty())
        {
            return str;
        }

        string result = str;
        size_t startPos = 0;

        while ((startPos = result.find(from, startPos)) != string::npos)
        {
            result.replace(startPos, from.length(), to);
            startPos += to.length();
        }

        return result;
    }

    string sanitizeFilename(const string& filename)
    {
        const string illegalChars = "\\/:*?\"<>|";
        string result = filename;

        for (char illegalChar : illegalChars)
        {
            string charStr(1, illegalChar);
            result = replaceAll(result, charStr, "_");
        }


        result = replaceAll(result, "..", "_");


        result = trim(result);


        size_t pos;
        while ((pos = result.find("__")) != string::npos)
        {
            result.replace(pos, 2, "_");
        }

        if (result.length() > 200) 
        {
            result = result.substr(0, 200);
            size_t lastUnderscore = result.find_last_of('_');
            if (lastUnderscore != string::npos)
            {
                result = result.substr(0, lastUnderscore);
            }
        }


        if (result.empty() || result == ".") 
        {
            result = "_";
        }


        return result;
    }


    string readMultilineInput()
    {
        cout << "Enter text below. Type ':w' on a new line by itself to save and finish:\n";
        cout << "----------------------------------------------------------------------\n";

        string input;
        string line;
        bool firstLine = true;
        string trimmedLine;

        while (true) 
        
        {
            if (!getline(cin, line)) 
            {
                if (cin.eof()) 
                {
                    cout << "\n----------------------------------------------------------------------\n";
                    cout << "Input finished (EOF detected).\n";
                    break;
                }
                else {
                    cin.clear();
                    cerr << "Input stream error. Aborting multiline input." << endl;
                    input.clear();
                    break;
                }
            }

            trimmedLine = trim(line);
            if (trimmedLine == ":w")
            {
                cout << "----------------------------------------------------------------------\n";
                cout << "Input finished.\n";
                break;
            }

            if (!firstLine)
            {
                input += "\n";
            }
            else
            {
                firstLine = false;
            }
            input += line;
        }


        return input;
    }


    string toLower(const string& str)
    {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return ::tolower(c); });
        return result;
    }

    string toUpper(const string& str)
    {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {return ::toupper(c); });
        return result;
    }

    void waitForEnter()
    {
        cout << "\nPress Enter to continue...";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin.rdbuf()->sungetc() != EOF && cin.peek() == '\n') 
        {
            cin.ignore();
        }


    }
}