#ifndef GRADETRACKER_H
#define GRADETRACKER_H

#include <string>
#include <vector>
#include <iostream>
#include "semester.h"
#include "../../include/auth.h"

using namespace std;

class GradeTracker
{
private:
    vector<Semester> semesters;
    string dataDirectoryPrefix;

public:
    GradeTracker();

    void addSemester(const string& name, const string& schoolYear, int numTerms);
    bool removeSemester(int index);
    size_t getSemesterCount() const;
    Semester& getSemester(int index);
    const Semester& getSemester(int index) const;
    void listSemesters() const;

    const vector<Semester>& getAllSemesters() const;
    void setAllSemesters(const vector<Semester>& loadedSemesters);


    bool saveData(const string& username) const;
    bool loadData(const string& username);
};


#endif // GRADETRACKER_H