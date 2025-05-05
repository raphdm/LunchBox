#include <iostream>
#include <cstdlib>
#include <fstream>
#include <direct.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../include/auth.h"
#include "../include/utils.h"
#include "../include/validation.h"

using namespace std;

bool createUserCredentialsDir()
{
    if (_mkdir("SavedData") != 0 && errno != EEXIST)
    {
        return false;
    }
    if (_mkdir("SavedData/UserCredentials") != 0 && errno != EEXIST)
    {
        return false;
    }
    return true;
}

User::User() : username(""), password(""), isLoggedIn(false) {}

string User::getUsername() const
{
    return this->username;
}

bool User::getLoginStatus() const
{
    return this->isLoggedIn;
}

bool User::login(const string& inputUsername, const string& inputPassword)
{
    if (!this->loadUserCredentials(inputUsername))
    {
        return false;
    }
    if (this->password == inputPassword)
    {
        this->isLoggedIn = true;
        return true;
    }
    return false;
}

void User::logout()
{
    this->isLoggedIn = false;
    this->username = "";
    this->password = "";
}

bool User::registerUser(const string& newUsername, const string& newPassword)
{
    if (!createUserCredentialsDir())
    {
        return false;
    }

    ifstream checkFile("SavedData/UserCredentials/UserCredentials_" + newUsername + ".txt");
    if (checkFile)
    {
        checkFile.close();
        return false;
    }

    this->username = newUsername;
    this->password = newPassword;

    return this->saveUserCredentials();
}

bool User::changePassword(const string& oldPassword, const string& newPassword)
{
    if (this->password == oldPassword)
    {
        this->password = newPassword;
        return this->saveUserCredentials();
    }
    return false;
}

bool User::loadUserCredentials(const string& username)
{
    ifstream inFile("SavedData/UserCredentials/UserCredentials_" + username + ".txt");
    if (!inFile)
    {
        return false;
    }

    string storedUsername;
    string storedPassword;

    getline(inFile, storedUsername);
    getline(inFile, storedPassword);

    inFile.close();

    if (storedUsername == username)
    {
        this->username = storedUsername;
        this->password = storedPassword;
        return true;
    }
    return false;
}

bool User::saveUserCredentials() const
{
    if (!createUserCredentialsDir()) {
        return false;
    }

    ofstream outFile("SavedData/UserCredentials/UserCredentials_" + username + ".txt");
    if (!outFile)
    {
        return false;
    }

    outFile << username << endl;
    outFile << password << endl;

    outFile.close();
    return true;
}

bool displayLoginMenu(User& user)
{
    int choice;
    string username, password, confirmPassword;

    do
    {
        system("cls");
        printUserAuthMenu();

        while (true)
        {
            cout << "Enter your choice: ";
            if (cin >> choice)
            {
                break;
            }
            else
            {
                clearInputStream();
                cout << "Invalid input. Please enter a number.\n";
                pauseScreen();
                system("cls");
                printUserAuthMenu();
            }
        }
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            int loginAttempts = 0;
            bool loginSuccess = false;

            while (loginAttempts < 3 && !loginSuccess)
            {
                system("cls");
                printLoginTitle();
                loginAttempts++;

                while (true)
                {
                    cout << "Username: ";
                    getline(cin, username);
                    if (username.empty())
                    {
                        system("cls");
                        printLoginTitle();
                        cout << "Username cannot be blank. Please try again.\n";
                    }
                    else
                    {
                        break;
                    }
                }

                while (true)
                {
                    cout << "Password: ";
                    getline(cin, password);
                    if (password.empty())
                    {
                        system("cls");
                        printLoginTitle();
                        cout << "Password cannot be blank. Please try again.\n";
                    }
                    else
                    {
                        break;
                    }
                }

                if (user.login(username, password))
                {
                    cout << "\nLogin Successful! Welcome, " << user.getUsername() << "!\n";
                    pauseScreen();
                    return true;
                }
                else
                {
                    cout << "\nLogin Failed! Invalid username or password." << endl;
                    if (loginAttempts < 3)
                    {
                        cout << "\nYou have " << (3 - loginAttempts) << " attempt(s) remaining." << endl;
                        pauseScreen();
                    }
                    else
                    {
                        cout << "\nMaximum login attempts reached." << endl;
                        cout << "Returning to main menu in 3 seconds..." << endl;
                        Sleep(3000);
                    }
                }
            }
            break;
        }
        case 2:
        {
            system("cls");
            printRegisterTitle();

            while (true)
            {
                cout << "Choose a username: ";
                getline(cin, username);
                if (username.empty())
                {
                    system("cls");
                    printRegisterTitle();
                    cout << "Username cannot be blank. Please try again.\n";
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Choose a password: ";
                getline(cin, password);
                if (!isValidPassword(password))
                {
                    system("cls");
                    printRegisterTitle();
                    cout << "Password must be between 8 and 16 characters. Please try again.\n";
                }
                else if (password.empty())
                {
                    system("cls");
                    printRegisterTitle();
                    cout << "Password cannot be blank. Please try again.\n";
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Confirm password: ";
                getline(cin, confirmPassword);
                if (confirmPassword.empty())
                {
                    system("cls");
                    printRegisterTitle();
                    cout << "Confirm password cannot be blank. Please try again.\n";
                }
                else if (password != confirmPassword)
                {
                    system("cls");
                    printRegisterTitle();
                    cout << "Choose a username: " << username << endl;
                    cout << "Choose a password: " << string(password.length(), '*') << endl;
                    cout << "\nPasswords do not match! Please try again.\n";
                }
                else
                {
                    break;
                }
            }

            if (user.registerUser(username, password))
            {
                cout << "\nRegistration Successful! You can now login." << endl;
            }
            else
            {
                cout << "\nRegistration Failed! Username already exists or an error occurred." << endl;
            }
            pauseScreen();
            break;
        }
        case 3:
            cout << "Exiting LunchBox, Goodbye!" << endl;
            return false;
        default:
            cout << "Invalid Choice! Please try again." << endl;
            pauseScreen();
        }
    } while (choice != 3);
    return false;
}