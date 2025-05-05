#include "../../include/AcademicRecords/gradeTracker.h"
#include "../../include/AcademicRecords/gradeStorage.h" // Changed include
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <fstream> // Added for ifstream check in loadData


using namespace std;


GradeTracker::GradeTracker() : dataDirectoryPrefix("SavedData/GradeData")
{
}

void GradeTracker::addSemester(const string& name, const string& schoolYear, int numTerms)
{
    semesters.push_back(Semester(name, schoolYear, numTerms));
}

bool GradeTracker::removeSemester(int index)
{
    if (index >= 0 && index < static_cast<int>(semesters.size()))
    {
        semesters.erase(semesters.begin() + index);
        return true;
    }
    return false;
}

size_t GradeTracker::getSemesterCount() const
{
    return semesters.size();
}

Semester& GradeTracker::getSemester(int index)
{
    if (index >= 0 && index < static_cast<int>(semesters.size()))
    {
        return semesters[index];
    }
    throw out_of_range("Semester index out of range in GradeTracker::getSemester");
}

const Semester& GradeTracker::getSemester(int index) const
{
    if (index >= 0 && index < static_cast<int>(semesters.size()))
    {
        return semesters[index];
    }
    throw out_of_range("Semester index out of range in GradeTracker::getSemester (const)");
}


void GradeTracker::listSemesters() const
{
    if (semesters.empty())
    {
        cout << "No semesters available." << endl;
        return;
    }

    cout << "\n--- AVAILABLE SEMESTERS ---" << endl;
    for (size_t i = 0; i < semesters.size(); ++i)
    {
        const Semester& sem = getSemester(static_cast<int>(i));
        cout << "[" << i + 1 << "] " << sem.getName()
            << " (" << sem.getSchoolYear() << ") - "
            << sem.getNumTerms() << " terms, "
            << sem.getSubjectCount() << " subjects" << endl;
    }
    cout << endl;
}

const vector<Semester>& GradeTracker::getAllSemesters() const {
    return semesters;
}

void GradeTracker::setAllSemesters(const vector<Semester>& loadedSemesters) {
    semesters = loadedSemesters;
}


bool GradeTracker::saveData(const string& username) const
{
    if (username.empty()) {
        cerr << "Error: Cannot save grade data without a username." << endl;
        return false;
    }

    if (!GradeStorage::Detail::createGradeDataDirectories(dataDirectoryPrefix, username)) {
        return false;
    }

    string userDirPath = dataDirectoryPrefix + "/" + username;
    string indexFilePath = userDirPath + "/index.txt";

    if (!GradeStorage::Detail::writeIndexFile(indexFilePath, semesters.size())) {
        return false;
    }

    GradeStorage::Detail::cleanupOldSemesterFiles(userDirPath, semesters.size());


    for (size_t i = 0; i < semesters.size(); ++i) {
        string semesterFilename = userDirPath + "/GradeRecords_" + to_string(i) + ".txt";
        if (!GradeStorage::Detail::writeSemesterFile(semesterFilename, semesters[i])) {
            return false;
        }
    }

    return true;
}

bool GradeTracker::loadData(const string& username)
{
    if (username.empty()) {
        cerr << "Error: Cannot load grade data without a username." << endl;
        return false;
    }

    if (!GradeStorage::Detail::createGradeDataDirectories(dataDirectoryPrefix, username)) {
        return false;
    }


    string userDirPath = dataDirectoryPrefix + "/" + username;
    string indexFilePath = userDirPath + "/index.txt";
    size_t semesterCount = 0;

    if (!GradeStorage::Detail::readIndexFile(indexFilePath, semesterCount)) {
        ifstream check(indexFilePath.c_str());
        if (!check) {
            cout << "No existing grade data found for user '" << username << "'. Starting fresh." << endl;
            semesters.clear();
            return true;
        }
        else {
            check.close();
            cerr << "Error reading existing index file: " << indexFilePath << endl;
            semesters.clear();
            return false;
        }
    }


    vector<Semester> loadedSemesters;
    loadedSemesters.reserve(semesterCount);
    bool success = true;

    for (size_t i = 0; i < semesterCount; ++i) {
        string semesterFilename = userDirPath + "/GradeRecords_" + to_string(i) + ".txt";
        try {
            loadedSemesters.push_back(GradeStorage::Detail::readSemesterFile(semesterFilename));
        }
        catch (const exception& e) {
            cerr << "Error loading semester file " << semesterFilename << ": " << e.what() << endl;
            success = false;
            break;
        }
    }

    if (success) {
        semesters = std::move(loadedSemesters);
        cout << "Grade data loaded successfully for user '" << username << "'." << endl;
        return true;
    }
    else {
        semesters.clear();
        return false;
    }
}