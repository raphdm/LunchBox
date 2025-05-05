#ifndef STUDENTPROFILE_H
#define STUDENTPROFILE_H
#include <string>
#include <vector>
#include "../auth.h"

using namespace std;

class StudentProfile
{
private:
    string username;
    string fullName;
    string studentID;
    string courseProgram;
    string yearLevel;

public:
    StudentProfile();
    void setUsername(const string& username);
    void setFullName(const string& name);
    void setStudentID(const string& id);
    void setCourseProgram(const string& course);
    void setYearLevel(const string& year);

    string getUsername() const;
    string getFullName() const;
    string getStudentID() const;
    string getCourseProgram() const;
    string getYearLevel() const;

    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);

    void displayInfo() const;
    bool hasData() const;
};

void addEditStudentInfo(User& user, StudentProfile& student);
void viewStudentInfo(const StudentProfile& student);
void deleteStudentProfile(StudentProfile& student, const User& user);

#endif