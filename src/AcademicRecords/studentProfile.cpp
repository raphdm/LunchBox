#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <direct.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../../include/AcademicRecords/studentProfile.h"
#include "../../include/utils.h"
#include "../../include/validation.h"
#include "../../include/auth.h"

using namespace std;

StudentProfile::StudentProfile() : username(""), fullName(""), studentID(""), courseProgram(""), yearLevel("") {}

void StudentProfile::setUsername(const string& username)
{
    this->username = username;
}

void StudentProfile::setFullName(const string& name)
{
    this->fullName = name;
}

void StudentProfile::setStudentID(const string& id)
{
    this->studentID = id;
}

void StudentProfile::setCourseProgram(const string& course)
{
    this->courseProgram = course;
}

void StudentProfile::setYearLevel(const string& year)
{
    this->yearLevel = year;
}

string StudentProfile::getUsername() const
{
    return this->username;
}

string StudentProfile::getFullName() const
{
    return this->fullName;
}

string StudentProfile::getStudentID() const
{
    return this->studentID;
}

string StudentProfile::getCourseProgram() const
{
    return this->courseProgram;
}

string StudentProfile::getYearLevel() const
{
    return this->yearLevel;
}

bool StudentProfile::saveToFile(const string& filename) const
{

    string filePath = "SavedData/StudentInformation/" + filename;
    ofstream outFile(filePath.c_str());
    if (!outFile) return false;

    outFile << this->username << endl;
    outFile << this->fullName << endl;
    outFile << this->studentID << endl;
    outFile << this->courseProgram << endl;
    outFile << this->yearLevel << endl;

    if (outFile.fail())
    {
        outFile.close();
        return false;
    }

    outFile.close();
    return true;
}

bool StudentProfile::loadFromFile(const string& filename)
{
    string filePath = "SavedData/StudentInformation/" + filename;
    ifstream inFile(filePath.c_str());
    if (!inFile) return false;

    string storedUsername;
    getline(inFile, storedUsername);

    if (storedUsername != this->username)
    {
        inFile.close();
        return false;
    }

    getline(inFile, this->fullName);
    getline(inFile, this->studentID);
    getline(inFile, this->courseProgram);
    getline(inFile, this->yearLevel);

    if (inFile.fail())
    {
        inFile.close();
        return false;
    }
    inFile.close();
    return true;
}

void StudentProfile::displayInfo() const
{
    cout << "+--------------------+" << endl;
    cout << "|   STUDENT PROFILE  |" << endl;
    cout << "+--------------------+" << endl;
    cout << "Full Name: " << this->fullName << endl;
    cout << "Student ID: " << this->studentID << endl;
    cout << "Course/Program: " << this->courseProgram << endl;
    cout << "Year Level: " << this->yearLevel << endl;
}

bool StudentProfile::hasData() const
{
    return !this->fullName.empty() && !this->studentID.empty();
}

void addEditStudentInfo(User& user, StudentProfile& student)
{
    system("cls");
    printAddOrEditInfoTitle();

    string name, id, course, year;
    bool isNewProfile = !student.hasData();


    if (!isNewProfile) {
        cout << "Current Information:" << endl;
        cout << "Full Name: " << student.getFullName() << endl;
        cout << "Student ID: " << student.getStudentID() << endl;
        cout << "Course/Program: " << student.getCourseProgram() << endl;
        cout << "Year Level: " << student.getYearLevel() << endl;
        cout << "\nEnter new values or leave blank to keep current values." << endl << endl;
    }


    while (true)
    {
        cout << "Enter Full Name: ";
        getline(cin, name);

        if (name.empty() && !isNewProfile) break;
        if (name.empty() && isNewProfile)
        {
            system("cls");
            printAddOrEditInfoTitle();
            cout << "Full Name cannot be blank for a new profile.\n";
            pauseScreen();
            system("cls");
            printAddOrEditInfoTitle();
        }
        else if (isValidName(name))
        {
            student.setFullName(name);
            break;
        }
        else
        {
            system("cls");
            printAddOrEditInfoTitle();
            cout << "Invalid Input! Name should not contain numbers.\n";
            pauseScreen();
            system("cls");
            printAddOrEditInfoTitle();
        }
    }


    while (true)
    {
        cout << "Enter Student ID: ";
        getline(cin, id);

        if (id.empty() && student.getStudentID().empty() && isNewProfile)
        {
            system("cls");
            printAddOrEditInfoTitle();
            cout << "Student ID cannot be blank for a new profile.\n";
            pauseScreen();
            system("cls");
            printAddOrEditInfoTitle();
        }
        else if (id.empty() && !student.getStudentID().empty()) break;
        else if (isValidStudentID(id))
        {
            student.setStudentID(id);
            break;
        }
        else
        {
            system("cls");
            printAddOrEditInfoTitle();
            cout << "Invalid Student ID Format! ID must be at least 5 characters.\n";
            pauseScreen();
            system("cls");
            printAddOrEditInfoTitle();
        }
    }


    while (true)
    {
        cout << "Enter Course/Program: ";
        getline(cin, course);
        if (!course.empty())
        {
            student.setCourseProgram(course);
            break;
        }
        else if (!student.getCourseProgram().empty()) break;
        else if (isNewProfile)
        {
            system("cls");
            printAddOrEditInfoTitle();
            cout << "Course/Program cannot be blank for a new profile.\n";
            pauseScreen();
            system("cls");
            printAddOrEditInfoTitle();
        }
        else break;
    }


    while (true)
    {
        cout << "Enter Year Level: ";
        getline(cin, year);
        if (!year.empty())
        {
            student.setYearLevel(year);
            break;
        }
        else if (!student.getYearLevel().empty()) break;
        else if (isNewProfile)
        {
            system("cls");
            printAddOrEditInfoTitle();
            cout << "Year Level cannot be blank for a new profile.\n";
            pauseScreen();
            system("cls");
            printAddOrEditInfoTitle();
        }
        else break;
    }

    string filename = "student_data_" + student.getUsername() + ".txt";
    if (student.saveToFile(filename))
    {
        cout << "\nProfile " << (isNewProfile ? "Created" : "Updated") << " Successfully!" << endl;
    }
    else
    {
        cout << "\nError Saving Profile! Please try again." << endl;
    }
    pauseScreen();
}

void viewStudentInfo(const StudentProfile& student)
{
    system("cls");
    printViewMyInfoTitle();

    if (student.hasData())
    {
        student.displayInfo();
    }
    else
    {
        cout << "\nNo student information found! Please add your information first.\n";
    }
    pauseScreen();
}

void deleteStudentProfile(StudentProfile& student, const User& user)
{
    if (!student.hasData()) {
        system("cls");
        printDeleteProfileTitle();
        cout << "No profile exists to delete." << endl;
        pauseScreen();
        return;
    }

    system("cls");
    printDeleteProfileTitle();

    cout << "Current profile to be deleted:" << endl;
    cout << "Full Name: " << student.getFullName() << endl;
    cout << "Student ID: " << student.getStudentID() << endl;
    cout << "Course/Program: " << student.getCourseProgram() << endl;
    cout << "Year Level: " << student.getYearLevel() << endl << endl;

    string confirm;
    while (true)
    {
        cout << "Are you sure you want to delete this profile? (y/n): ";
        getline(cin, confirm);

        if (confirm == "y" || confirm == "Y")
        {
            string filename = "student_data_" + student.getUsername() + ".txt";
            if (DeleteFileA(("SavedData/StudentInformation/" + filename).c_str()))
            {
                cout << "\nProfile deleted successfully!" << endl;
                StudentProfile emptyStudent;
                emptyStudent.setUsername(student.getUsername());
                student = emptyStudent;
            }
            else
            {
                cout << "\nError deleting profile. File may not exist." << endl;
            }
            pauseScreen();
            break;
        }
        else if (confirm == "n" || confirm == "N")
        {
            cout << "\nDeletion cancelled." << endl;
            pauseScreen();
            break;
        }
        else
        {
            system("cls");
            printDeleteProfileTitle();
            cout << "\nInvalid input! Please enter 'y' or 'n'." << endl;
        }
    }
}