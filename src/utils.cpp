#include "../include/utils.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <vector>
#include <string>
#include <functional>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

void printUserAuthMenu()
{
    cout << "=================================" << endl;
    cout << "|           LunchBox            |" << endl;
    cout << "|       Student Toolkit         |" << endl;
    cout << "=================================" << endl;
    cout << endl;
    cout << "[1] Login" << endl;
    cout << "[2] Register" << endl;
    cout << "[3] Exit" << endl;
    cout << endl;
}

void printLoginTitle()
{
    cout << "=================================" << endl;
    cout << "|            Login              |" << endl;
    cout << "=================================" << endl;
    cout << endl;
}

void printRegisterTitle()
{
    cout << "=================================" << endl;
    cout << "|        Registration           |" << endl;
    cout << "=================================" << endl;
    cout << endl;
}

void printAcademicRecordsSubMenu()
{
    cout << "=======================" << endl;
    cout << "  My Academic Records  " << endl;
    cout << "=======================" << endl;
    cout << endl;
    cout << "[1] Add / Edit My Info" << endl;
    cout << "[2] View My Info" << endl;
    cout << "[3] Delete My Profile" << endl;
    cout << "[4] Grade Tracker" << endl;
    cout << "[5] Back to Main Menu" << endl;
    cout << endl;
}


void printDeleteProfileTitle()
{
    cout << "=======================" << endl;
    cout << "   Delete My Profile   " << endl;
    cout << "=======================" << endl;
    cout << endl;
}

void printAddOrEditInfoTitle()
{
    cout << "======================" << endl;
    cout << "  Add / Edit My Info  " << endl;
    cout << "======================" << endl;
    cout << endl;
}

void printViewMyInfoTitle()
{
    cout << "=======================" << endl;
    cout << "      View My Info     " << endl;
    cout << "=======================" << endl;
    cout << endl;
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.clear();
    cin.get();
}

void clearInputStream()
{
    cin.clear();
    cin.ignore(1000, '\n');
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayHeader(const string& title)
{
    clearScreen();
    cout << "===================================" << endl;
    int padding = (35 - title.length()) / 2;
    cout << string(padding > 0 ? padding : 0, ' ') << title << endl;
    cout << "===================================" << endl << endl;
}

void displayMenuOptions(const vector<string>& options)
{
    for (size_t i = 0; i < options.size(); ++i)
    {
        if (!options.empty())
        {
            cout << "[" << i + 1 << "] " << options[i] << endl;
        }
    }
    cout << endl;
}

int getIntInput(const string& prompt, int min, int max)
{
    int input;
    int attempts = 0;
    const int MAX_ATTEMPTS = 1;
    int failure_signal = min - 1;

    while (attempts < MAX_ATTEMPTS)
    {
        cout << prompt;
        if (cin >> input)
        {
            char nextChar = cin.peek();
            if (isspace(nextChar) || nextChar == EOF)
            {
                if (input >= min && input <= max)
                {
                    clearInputStream();
                    return input;
                }
                else
                {
                    cout << "Input out of range. Please enter a value between " << min << " and " << max << ".\n" << endl;
                      clearInputStream();
                }
            }
            else
            {
                cout << "Invalid input. Please enter only a whole number.\n" << endl;
                clearInputStream();
            }
        }
        else
        {
            cout << "Invalid input. Please enter a whole number.\n" << endl;

            clearInputStream();
        }
        attempts++;
    }
	cout << "Returning..." << endl;
    Sleep(550); 
    return failure_signal;
}

double getDoubleInput(const string& prompt, double min, double max)
{
    double input;
    int attempts = 0;
    const int MAX_ATTEMPTS = 1;
    double failure_signal = min - 1.0;

    while (attempts < MAX_ATTEMPTS)
    {
        cout << prompt;
        if (cin >> input)
        {
            char nextChar = cin.peek();
            if (isspace(nextChar) || nextChar == EOF)
            {
                if (input >= min && input <= max)
                {
                    clearInputStream();
                    return input;
                }
                else
                {
                    cout << "Input out of range. Please enter a value between " << fixed << setprecision(2) << min << " and " << max << ".\n" << endl;
                    clearInputStream();
                }
            }
            else 
            {
                cout << "Invalid input. Please enter only a number.\n" << endl;
                clearInputStream();
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n" << endl;
            // Sleep removed
            clearInputStream();
        }
        attempts++;
    }

    cout << "Returning..." << endl;
    Sleep(550); 
    return failure_signal;
}

string getStringInput(const string& prompt, bool allowEmpty, int minLength, int maxLength)
{
    string input;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    string failure_signal = "";

    while (attempts < MAX_ATTEMPTS)
    {
        cout << prompt;
        getline(cin, input);

        bool isEmpty = true;
        for (size_t i = 0; i < input.length(); ++i)
        {
            if (!isspace(static_cast<unsigned char>(input[i]))) 
            {
                isEmpty = false;
                break;
            }
        }

        if (!allowEmpty && (input.empty() || isEmpty))
        {
            cout << "Input cannot be empty.\n" << endl;
        }
        else if (static_cast<int>(input.length()) < minLength)
        {
            cout << "Input must be at least " << minLength << " characters long.\n" << endl;
        }
        else if (static_cast<int>(input.length()) > maxLength)
        {
            cout << "Input must be no more than " << maxLength << " characters long.\n" << endl;
        }
        else
        {
            return input;
        }

        attempts++;
    }

    cout << "Returning...\n" << endl;
    Sleep(550); 
    return failure_signal;
}

bool getYesNoInput(const string& prompt)
{
    char inputChar;
    int attempts = 0;
    const int MAX_ATTEMPTS = 1;

    while (attempts < MAX_ATTEMPTS)
    {
        cout << prompt << " (y/n): ";

        if (cin >> inputChar)
        {
            clearInputStream();
            inputChar = tolower(inputChar);

            if (inputChar == 'y')
            {
                return true;
            }
            else if (inputChar == 'n')
            {
                return false;
            }
            else
            {
                cout << "Invalid input. Please enter 'y' or 'n'.\n" << endl;
            }
        }
        else
        {
            cout << "Invalid input stream state. Please try again.\n" << endl;
            clearInputStream();
        }

        attempts++;
    }
    cout << "Defaulting to 'No'...\n" << endl;
    Sleep(550); 
    return false;
}