#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../include/auth.h"
#include "../include/utils.h"
#include "../include/FeatureModule.h"

using namespace std;

namespace
{
    void printDynamicMainMenu(const string& username, const vector<FeatureModule*>& modules)
    {
        cout << "=================================" << endl;
        cout << "|      Welcome to LunchBox!     |" << endl;
        cout << "|       A Student Toolkit       |" << endl;
        cout << "=================================" << endl;
        cout << endl;
        cout << "[ Logged in as: " << username << " ]" << endl;
        cout << endl;

        for (size_t i = 0; i < modules.size(); ++i)
        {
            cout << "[" << (i + 1) << "] " << modules[i]->getName() << endl;
        }

        cout << "[" << (modules.size() + 1) << "] Logout" << endl;
        cout << "[" << (modules.size() + 2) << "] Exit" << endl;
        cout << endl;
    }
}

void displayMainMenu(User& user, const vector<FeatureModule*>& modules)
{
    int choice;
    int logoutOption = static_cast<int>(modules.size()) + 1;
    int exitOption = static_cast<int>(modules.size()) + 2;

    do
    {
        system("cls");
        printDynamicMainMenu(user.getUsername(), modules);

        while (true)
        {
            cout << "Enter your choice (1-" << exitOption << "): ";
            if (cin >> choice)
            {
                if (cin.peek() == '\n' && choice >= 1 && choice <= exitOption) 
                {
                    cin.ignore();
                    break;
                }
                else 
                {
                    clearInputStream();
                    system("cls");
                    printDynamicMainMenu(user.getUsername(), modules);
                    cout << "Invalid input! Please enter a number between 1 and " << exitOption << "." << endl;
                    pauseScreen();
                    system("cls");
                    printDynamicMainMenu(user.getUsername(), modules);
                }
            }
            else
            {
                clearInputStream();
                system("cls");
                printDynamicMainMenu(user.getUsername(), modules);
                cout << "Invalid input! Please enter a number." << endl;
                pauseScreen();
                system("cls");
                printDynamicMainMenu(user.getUsername(), modules);
            }
        }

        if (choice >= 1 && choice < logoutOption)
        {
            modules[choice - 1]->run(user);
        }
        else if (choice == logoutOption)
        {
            char confirm;
            cout << "Are you sure you want to log out? (y/n): ";
            while (!(cin >> confirm) || (tolower(confirm) != 'y' && tolower(confirm) != 'n')) 
            {
                cout << "Invalid input. Please enter 'y' or 'n': ";
                clearInputStream();
            }
            cin.ignore();


            if (tolower(confirm) == 'y')
            {
                user.logout();
                cout << "You have been logged out." << endl;
                pauseScreen();

                if (!displayLoginMenu(user))
                {
                    choice = exitOption;
                    cout << "Exiting LunchBox. Goodbye!" << endl;
                }
            }
        }
        else if (choice == exitOption)
        {
            cout << "Exiting LunchBox. Goodbye!" << endl;
            Sleep(1000);
        }

    } while (choice != exitOption);
}