#ifndef SEMESTER_H
#define SEMESTER_H

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

const double PASSING_MARK = 75.0;

class Grade
{
private:
    double value;
    bool isSet;

public:
    Grade();

    void setValue(double val);
    double getValue() const;
    bool getIsSet() const;
    bool isPassing() const;
    string getStatus() const;
};

class Subject
{
private:
    string name;
    vector<Grade> termGrades;

public:
    Subject(const string& subjectName, int numTerms);

    string getName() const;
    void setName(const string& newName);
    void setGrade(int term, double grade);
    Grade getGrade(int term) const;
    double calculateAverage() const;
    bool isOverallPassing() const;
    string getOverallStatus() const;
    size_t getNumTerms() const;
};

class Semester
{
private:
    string name;
    string schoolYear;
    int numTerms;
    vector<Subject> subjects;

public:
    Semester(const string& semName, const string& year, int terms);

    string getName() const;
    void setName(const string& newName);
    string getSchoolYear() const;
    void setSchoolYear(const string& newYear);
    int getNumTerms() const;
    size_t getSubjectCount() const;

    void addSubject(const string& subjectName);
    void removeSubject(int index);
    Subject& getSubject(int index);

    double calculateTermAverage(int term) const;
    string getTermStatus(int term) const;

    void displayGrades() const;
    bool saveToFile(const string& filename) const;
    static Semester loadFromFile(const string& filename);
};

#endif