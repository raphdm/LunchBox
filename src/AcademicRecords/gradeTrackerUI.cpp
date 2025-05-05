#include "../../include/AcademicRecords/gradeTrackerUI.h"
#include "../../include/AcademicRecords/gradeTracker.h"
#include "../../include/utils.h"
#include "../../include/validation.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

void editSubject(Subject& subject)
{
    displayHeader("EDIT SUBJECT: " + subject.getName());
    string newName = getStringInput("Enter new name (or press Enter to keep current): ", true, 0, 50);
    if (!newName.empty() && isValidSubjectName(newName))
    {
        subject.setName(newName);
        cout << "Subject name updated successfully!" << endl;
    }
    else if (!newName.empty())
    {
        cout << "Invalid subject name provided. Name not changed." << endl;
    }
    else
    {
        cout << "No changes made to subject name." << endl;
    }
    pauseScreen();
}

void editSemesterDetails(Semester& semester)
{
    bool running = true;
    vector<string> options = {
        "Change Semester Name",
        "Change School Year",
        "Return to Semester Menu"
    };
    while (running)
    {
        displayHeader("EDIT SEMESTER: " + semester.getName() + " - " + semester.getSchoolYear());
        displayMenuOptions(options);
        int choice = getIntInput("Choose an option: ", 1, static_cast<int>(options.size()));
        if (choice < 1)
        {
            cout << "Returning to semester menu due to invalid input." << endl;
            running = false;
            break;
        }
        switch (choice)
        {
        case 1:
        {
            string newName = getStringInput("Enter new semester name: ", false, 1, 30);
            if (!newName.empty())
            {
                semester.setName(newName);
                cout << "Semester name updated successfully!" << endl;
            }
            else
            {
                cout << "Operation canceled (invalid input or max attempts reached)." << endl;
            }
            pauseScreen();
            break;
        }
        case 2:
        {
            string newYear;
            bool validYear = false;
            int attempts = 0;
            const int MAX_ATTEMPTS = 3;
            while (attempts < MAX_ATTEMPTS && !validYear)
            {
                newYear = getStringInput("Enter new school year (YYYY-YYYY): ", false, 9, 9);
                if (!newYear.empty() && isValidSchoolYear(newYear))
                {
                    validYear = true;
                }
                else if (!newYear.empty())
                {
                    cout << "Invalid format. Please use YYYY-YYYY." << endl;
                }
                else
                {
                    break;
                }
                attempts++;
            }
            if (validYear)
            {
                semester.setSchoolYear(newYear);
                cout << "School year updated successfully!" << endl;
            }
            else
            {
                cout << "Operation canceled (invalid input or max attempts reached)." << endl;
            }
            pauseScreen();
            break;
        }
        case 3:
            running = false;
            break;
        }
    }
}


void manageSemester(Semester& semester)
{
    int choice;
    bool running = true;
    vector<string> options =
    {
        "Add Subject",
        "Remove Subject",
        "Edit Subject Name",
        "Update Grades for Subject",
        "View Grade Report",
        "Edit Semester Details",
        "Return to Main Grade Menu"
    };
    while (running)
    {
        displayHeader(semester.getName() + " SEMESTER (" + semester.getSchoolYear() + ")");
        displayMenuOptions(options);
        choice = getIntInput("Choose an option: ", 1, static_cast<int>(options.size()));
        if (choice < 1)
        {
            cout << "Returning to main grade menu due to invalid input." << endl;
            running = false;
            break;
        }
        switch (choice)
        {
        case 1: {
            displayHeader("ADD SUBJECT to " + semester.getName());
            string subjectName;
            bool validName = false;
            int attempts = 0;
            const int MAX_ATTEMPTS = 3;
            while (attempts < MAX_ATTEMPTS && !validName)
            {
                subjectName = getStringInput("Enter subject name: ", false, 1, 50);
                if (!subjectName.empty() && isValidSubjectName(subjectName))
                {
                    validName = true;
                }
                else if (!subjectName.empty())
                {
                    cout << "Invalid subject name (cannot be empty or just whitespace)." << endl;
                }
                else
                {
                    break;
                }
                attempts++;
            }
            if (validName)
            {
                semester.addSubject(subjectName);
                cout << "Subject '" << subjectName << "' added successfully!" << endl;
            }

            break;
        }
        case 2:
        {
            displayHeader("REMOVE SUBJECT from " + semester.getName());
            if (semester.getSubjectCount() == 0) {
                cout << "No subjects to remove." << endl;
            }
            else
            {
                cout << "--- SUBJECTS ---" << endl;
                for (size_t i = 0; i < semester.getSubjectCount(); ++i)
                {
                    try
                    {
                        cout << "[" << i + 1 << "] " << semester.getSubject(static_cast<int>(i)).getName() << endl;
                    }
                    catch (const out_of_range&)
                    {
                        cerr << "Error accessing subject at index " << i << endl;
                    }
                }
                cout << "[0] Cancel" << endl << endl;
                int subjectIndex = getIntInput("Select subject to remove: ", 0, static_cast<int>(semester.getSubjectCount())) - 1;
                if (subjectIndex >= 0 && subjectIndex < static_cast<int>(semester.getSubjectCount()))
                {
                    try
                    {
                        string subjectNameToRemove = semester.getSubject(subjectIndex).getName();
                        if (getYesNoInput("Are you sure you want to remove '" + subjectNameToRemove + "'?")) {
                            semester.removeSubject(subjectIndex);
                            cout << "Subject removed successfully!" << endl;
                        }
                        else
                        {
                            cout << "Operation cancelled." << endl;
                        }
                    }
                    catch (const out_of_range& e)
                    {
                        cerr << "Error accessing subject to remove: " << e.what() << ". Operation cancelled." << endl;
                    }
                }
                else if (subjectIndex == -1)
                {
                    cout << "Operation cancelled." << endl;
                }
            }
            pauseScreen();
            break;
        }
        case 3:
        {
            displayHeader("EDIT SUBJECT NAME in " + semester.getName());
            if (semester.getSubjectCount() == 0) {
                cout << "No subjects available to edit." << endl;
                pauseScreen();
            }
            else
            {
                cout << "--- SUBJECTS ---" << endl;
                for (size_t i = 0; i < semester.getSubjectCount(); ++i)
                {
                    try
                    {
                        cout << "[" << i + 1 << "] " << semester.getSubject(static_cast<int>(i)).getName() << endl;
                    }
                    catch (const out_of_range&)
                    {
                        cerr << "Error accessing subject at index " << i << endl;
                    }
                }
                cout << "[0] Cancel" << endl << endl;
                int subjectIndex = getIntInput("Select subject to edit: ", 0, static_cast<int>(semester.getSubjectCount())) - 1;
                if (subjectIndex >= 0 && subjectIndex < static_cast<int>(semester.getSubjectCount()))
                {
                    try
                    {
                        editSubject(semester.getSubject(subjectIndex));
                    }
                    catch (const out_of_range& e)
                    {
                        cerr << "Error accessing subject to edit: " << e.what() << ". Operation cancelled." << endl;
                        pauseScreen();
                    }
                }
                else if (subjectIndex == -1)
                {
                    cout << "Operation cancelled." << endl;
                    pauseScreen();
                }
                else
                {
                    pauseScreen();
                }
            }
            break;
        }
        case 4:
        {
            displayHeader("UPDATE GRADES for " + semester.getName());
            if (semester.getSubjectCount() == 0)
            {
                cout << "No subjects available. Add subjects first." << endl;
            }
            else
            {
                cout << "--- SUBJECTS ---" << endl;
                for (size_t i = 0; i < semester.getSubjectCount(); ++i)
                {
                    try
                    {
                        cout << "[" << i + 1 << "] " << semester.getSubject(static_cast<int>(i)).getName() << endl;
                    }
                    catch (const out_of_range&)
                    {
                        cerr << "Error accessing subject at index " << i << endl;
                    }
                }
                cout << "[0] Cancel" << endl << endl;
                int subjectIndex = getIntInput("Select subject to update grades for: ", 0, static_cast<int>(semester.getSubjectCount())) - 1;
                if (subjectIndex >= 0 && subjectIndex < static_cast<int>(semester.getSubjectCount()))
                {
                    try
                    {
                        Subject& subject = semester.getSubject(subjectIndex);
                        displayHeader("UPDATING GRADES FOR: " + subject.getName());
                        bool gradeUpdated = false;
                        int numTerms = semester.getNumTerms();
                        for (int term = 0; term < numTerms; ++term)
                        {
                            Grade currentGrade = subject.getGrade(term);
                            string prompt;
                            double grade = -2.0;
                            if (currentGrade.getIsSet())
                            {
                                prompt = "Enter new grade for Term " + to_string(term + 1) +
                                    " (current: " + to_string(currentGrade.getValue()) +
                                    ", or -1 to skip): ";
                                grade = getDoubleInput(prompt, -1.0, 100.0);
                            }
                            else
                            {
                                prompt = "Enter grade for Term " + to_string(term + 1) +
                                    " (0-100, or -1 to skip): ";
                                grade = getDoubleInput(prompt, -1.0, 100.0);
                            }
                            if (grade < -1.0)
                            {
                                cout << "Invalid input for Term " << term + 1 << ". Skipping remaining terms for this subject." << endl;
                                break;
                            }
                            if (grade >= 0)
                            {
                                subject.setGrade(term, grade);
                                cout << "Term " << term + 1 << " grade set to " << fixed << setprecision(2) << grade
                                    << ". Status: " << subject.getGrade(term).getStatus() << endl;
                                gradeUpdated = true;
                            }
                            else
                            {
                                cout << "Skipping Term " << term + 1 << "." << endl;
                            }
                        }
                        if (gradeUpdated)
                        {
                            cout << "\nGrade updates for " << subject.getName() << " complete." << endl;
                        }
                        else
                        {
                            cout << "\nNo grades were updated for " << subject.getName() << "." << endl;
                        }
                    }
                    catch (const out_of_range&)
                    {
                        cerr << "Error accessing subject to update grades. Operation cancelled." << endl;
                    }
                }
                else if (subjectIndex == -1)
                {
                    cout << "Operation cancelled." << endl;
                }
            }
            pauseScreen();
            break;
        }
        case 5:
        {
            try
            {
                semester.displayGrades();
            }
            catch (const exception& e)
            {
                cerr << "An error occurred while displaying grades: " << e.what() << endl;
                pauseScreen();
            }
            break;
        }
        case 6:
        {
            editSemesterDetails(semester);
            break;
        }
        case 7:
            running = false;
            break;
        }
    }
}


void manageGradeTracking(User& user)
{
    GradeTracker tracker;
    int choice;
    bool running = true;
    if (!tracker.loadData(user.getUsername()))
    {
        cerr << "Critical error loading grade data. Please check file integrity or contact support." << endl;
        tracker = GradeTracker();
    }
    else
    {
    }
    pauseScreen();
    vector<string> mainOptions =
    {
        "Add New Semester",
        "Manage Existing Semester",
        "Remove Semester",
        "Save Current Data",
        "Return to Academic Records Menu"
    };
    vector<string> semesterTypeOptions =
    {
       "First Semester",
       "Second Semester",
       "Midyear/Summer",
       "Custom Name"
    };
    while (running)
    {
        displayHeader("Grade Tracker Menu");
        displayMenuOptions(mainOptions);
        choice = getIntInput("Choose an option: ", 1, static_cast<int>(mainOptions.size()));
        if (choice < 1)
        {
            cout << "Returning to Academic Records menu due to invalid input." << endl;
            running = false;
            break;
        }
        switch (choice)
        {
            case 1:
            {
                displayHeader("ADD NEW SEMESTER");
                displayMenuOptions(semesterTypeOptions);
                int semTypeChoice = getIntInput("Choose semester type: ", 1, static_cast<int>(semesterTypeOptions.size()));
                if (semTypeChoice < 1) { cout << "Operation canceled." << endl; pauseScreen(); break; }
                string semName;
                if (semTypeChoice == static_cast<int>(semesterTypeOptions.size()))
                {
                    semName = getStringInput("Enter custom semester name: ", false, 1, 30);
                    if (semName.empty() || !isValidSubjectName(semName)) {
                        cout << "Invalid or empty custom name. Operation canceled." << endl; pauseScreen(); break;
                    }
                }
                else {
                    semName = semesterTypeOptions[semTypeChoice - 1];
                }
                string schoolYear;
                bool validYear = false;
                int yearAttempts = 0;
                while (yearAttempts < 3 && !validYear) {
                    schoolYear = getStringInput("Enter school year (YYYY-YYYY): ", false, 9, 9);
                    if (!schoolYear.empty() && isValidSchoolYear(schoolYear)) { validYear = true; }
                    else if (!schoolYear.empty()) { cout << "Invalid format. Please use YYYY-YYYY." << endl; }
                    else { break; }
                    yearAttempts++;
                }
                if (!validYear) { cout << "Invalid school year entered. Operation canceled." << endl; pauseScreen(); break; }
                int numTerms = getIntInput("Enter number of grading terms/periods (1-4): ", 1, 4);
                if (numTerms < 1) { cout << "Invalid number of terms. Operation canceled." << endl; pauseScreen(); break; }
                tracker.addSemester(semName, schoolYear, numTerms);
                cout << "\nSemester '" << semName << " (" << schoolYear << ")' with " << numTerms << " term(s) added successfully!" << endl;
                pauseScreen();
                break;
            }
            case 2:
            {
                displayHeader("MANAGE SEMESTER");
                if (tracker.getSemesterCount() == 0) {
                    cout << "No semesters available. Please add a semester first." << endl;
                    pauseScreen();
                }
                else {
                    tracker.listSemesters();
                    cout << "[0] Cancel" << endl << endl;
                    int semIndex = getIntInput("Select semester to manage: ", 0, static_cast<int>(tracker.getSemesterCount())) - 1;
                    if (semIndex >= 0 && semIndex < static_cast<int>(tracker.getSemesterCount())) {
                        try {
                            manageSemester(tracker.getSemester(semIndex));
                        }
                        catch (const out_of_range& e) {
                            cerr << "Error accessing selected semester: " << e.what() << ". Returning to menu." << endl; pauseScreen();
                        }
                    }
                    else if (semIndex == -1) { cout << "Operation cancelled." << endl; pauseScreen(); }
                    else { pauseScreen(); }
                }
                break;
            }
            case 3:
            {
                displayHeader("REMOVE SEMESTER");
                if (tracker.getSemesterCount() == 0) {
                    cout << "No semesters available to remove." << endl;
                }
                else {
                    tracker.listSemesters();
                    cout << "[0] Cancel" << endl << endl;
                    int semIndex = getIntInput("Select semester to remove: ", 0, static_cast<int>(tracker.getSemesterCount())) - 1;
                    if (semIndex >= 0 && semIndex < static_cast<int>(tracker.getSemesterCount())) {
                        try {
                            string semNameToRemove = tracker.getSemester(semIndex).getName();
                            if (getYesNoInput("Are you sure you want to remove '" + semNameToRemove + "'?")) {
                                if (tracker.removeSemester(semIndex)) {
                                    cout << "Semester '" << semNameToRemove << "' removed successfully!" << endl;
                                }
                                else {
                                    cout << "Failed to remove semester." << endl;
                                }
                            }
                            else {
                                cout << "Operation cancelled." << endl;
                            }
                        }
                        catch (const out_of_range& e) {
                            cerr << "Error accessing selected semester: " << e.what() << ". Operation cancelled." << endl;
                        }
                    }
                    else if (semIndex == -1) {
                        cout << "Operation cancelled." << endl;
                    }
                }
                pauseScreen();
                break;
            }
            case 4:
            {
                displayHeader("SAVE GRADE DATA");
                if (tracker.saveData(user.getUsername())) {
                    cout << "Grade data saved successfully!" << endl;
                }
                else {
                    cout << "Failed to save grade data." << endl;
                }
                pauseScreen();
                break;
            }
            case 5:
            {
                if (getYesNoInput("Save current changes before returning?")) {
                    displayHeader("SAVE GRADE DATA");
                    if (tracker.saveData(user.getUsername())) {
                        cout << "Grade data saved successfully!" << endl;
                    }
                    else {
                        cout << "Failed to save grade data." << endl;
                    }
                    pauseScreen();
                }
                cout << "\nReturning to Academic Records Menu..." << endl;
                running = false;
                Sleep(600);
                break;
            }
        }
    }
}