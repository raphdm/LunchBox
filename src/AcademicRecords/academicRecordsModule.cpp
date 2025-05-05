#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <direct.h>
#include <cerrno>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../include/AcademicRecords/AcademicRecordsModule.h"
#include "../../include/AcademicRecords/StudentProfile.h"
#include "../../include/AcademicRecords/gradeTrackerUI.h" 
#include "../../include/auth.h"
#include "../../include/utils.h"
#include "../../include/validation.h"

using namespace std;

namespace
{
    bool createStudentInfoDir()
    {
        _mkdir("SavedData");

        if (_mkdir("SavedData/StudentInformation") != 0 && errno != EEXIST)
        {
            cerr << "Error creating directory: SavedData/StudentInformation. Error code: " << errno << endl;
            return false;
        }
        return true;
    }
}

string AcademicRecordsModule::getName() const
{
    return "Academic Records";
}

void AcademicRecordsModule::run(User& user)
{
    StudentProfile currentStudent;
    currentStudent.setUsername(user.getUsername());
    string profileFilename = "student_data_" + currentStudent.getUsername() + ".txt";

    createStudentInfoDir();

    if (!currentStudent.loadFromFile(profileFilename))
    {
        cout << "\nNo student profile found for " << user.getUsername() << "." << endl;
        cout << "You can create one using the 'Add / Edit My Info' option." << endl;
        pauseScreen();
    }

    int academicChoice;
    int minChoice = 1;
    int maxChoice = 5;
    int failure_signal = minChoice - 1;

    do
    {
        system("cls");
        printAcademicRecordsSubMenu(); 

        academicChoice = getIntInput("Enter your choice (1-5): ", minChoice, maxChoice);

        switch (academicChoice)
        {
        case 1:
            addEditStudentInfo(user, currentStudent);
            break;
        case 2:
            viewStudentInfo(currentStudent); 
            break;
        case 3:
            deleteStudentProfile(currentStudent, user); 
            break;
        case 4:
            manageGradeTracking(user); 
            break;
        case 5:
            cout << "\nReturning to Main Menu..." << endl;
            Sleep(600);
            break;
        }
    } while (academicChoice != maxChoice);
}