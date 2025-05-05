#include "../../include/AcademicRecords/semester.h"
#include "../../include/utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <direct.h>
#include <cerrno>
#include <stdexcept>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Grade::Grade() : value(0.0), isSet(false) {}

void Grade::setValue(double val)
{
    value = val;
    isSet = true;
}

double Grade::getValue() const
{
    return value;
}

bool Grade::getIsSet() const
{
    return isSet;
}

bool Grade::isPassing() const
{
    return isSet && value >= PASSING_MARK;
}

string Grade::getStatus() const
{
    if (!isSet) return "N/A";
    return (isPassing()) ? "Passed" : "Failed";
}


Subject::Subject(const string& subjectName, int numTerms) : name(subjectName)
{
    if (numTerms > 0) 
    {
        termGrades.resize(numTerms);
    }
}

string Subject::getName() const
{
    return name;
}

void Subject::setName(const string& newName)
{
    name = newName;
}

void Subject::setGrade(int term, double grade)
{
    if (term >= 0 && term < static_cast<int>(termGrades.size()))
    {
        termGrades[term].setValue(grade);
    }
}

Grade Subject::getGrade(int term) const
{
    if (term >= 0 && term < static_cast<int>(termGrades.size()))
    {
        return termGrades[term];
    }

    return Grade();
}

double Subject::calculateAverage() const
{
    double sum = 0.0;
    int count = 0;

    for (size_t i = 0; i < termGrades.size(); ++i)
    {
        if (termGrades[i].getIsSet())
        {
            sum += termGrades[i].getValue();
            count++;
        }
    }
    return (count > 0) ? (sum / count) : 0.0;
}

bool Subject::isOverallPassing() const
{

    int count = 0;
    for (size_t i = 0; i < termGrades.size(); ++i) 
    {
        if (termGrades[i].getIsSet()) 
        {
            count++;
            break;
        }
    }
    if (count == 0) return false;

    return calculateAverage() >= PASSING_MARK;
}

string Subject::getOverallStatus() const
{

    int count = 0;
    for (size_t i = 0; i < termGrades.size(); ++i) 
    {
        if (termGrades[i].getIsSet()) 
        {
            count++;
            break;
        }
    }
    if (count == 0) return "N/A";

    return isOverallPassing() ? "Passed" : "Failed";
}

size_t Subject::getNumTerms() const
{
    return termGrades.size();
}

Semester::Semester(const string& semName, const string& year, int terms)
    : name(semName), schoolYear(year), numTerms(terms > 0 ? terms : 0)
{

}

string Semester::getName() const
{
    return name;
}

void Semester::setName(const string& newName)
{

    name = newName;
}

string Semester::getSchoolYear() const
{
    return schoolYear;
}

void Semester::setSchoolYear(const string& newYear)
{

    schoolYear = newYear;
}

int Semester::getNumTerms() const
{
    return numTerms;
}

size_t Semester::getSubjectCount() const
{
    return subjects.size();
}

void Semester::addSubject(const string& subjectName)
{


    if (numTerms > 0) 
    {
        subjects.push_back(Subject(subjectName, numTerms));
    }

}

void Semester::removeSubject(int index)
{
    if (index >= 0 && index < static_cast<int>(subjects.size()))
    {
        subjects.erase(subjects.begin() + index);
    }

}

Subject& Semester::getSubject(int index)
{

    if (index >= 0 && static_cast<size_t>(index) < subjects.size())
    {
        return subjects[index];
    }
    throw out_of_range("Subject index out of range in Semester::getSubject");
}

double Semester::calculateTermAverage(int term) const
{
    if (term < 0 || term >= numTerms) 
    {
        return 0.0;
    }

    double sum = 0.0;
    int count = 0;

    for (size_t i = 0; i < subjects.size(); ++i)
    {

        Grade grade = subjects[i].getGrade(term);
        if (grade.getIsSet())
        {
            sum += grade.getValue();
            count++;
        }
    }
    return (count > 0) ? (sum / count) : 0.0;
}

string Semester::getTermStatus(int term) const
{
    if (term < 0 || term >= numTerms) 
    {
        return "N/A";
    }
    double avg = calculateTermAverage(term);


    int count = 0;
    for (size_t i = 0; i < subjects.size(); ++i) 
    {
        if (subjects[i].getGrade(term).getIsSet())
        {
            count++;
            break;
        }
    }
    if (count == 0) return "N/A";

    return (avg >= PASSING_MARK) ? "Passed" : "Failed";
}

void Semester::displayGrades() const
{

    displayHeader("GRADE REPORT: " + name + " - " + schoolYear);

    if (subjects.empty()) 
    {
        cout << "No subjects added for this semester yet." << endl;
        pauseScreen();
        return;
    }

    cout << left << setw(30) << "SUBJECT";
    for (int i = 0; i < numTerms; ++i)
    {
        cout << setw(10) << ("Term " + to_string(i + 1));
    }
    cout << setw(10) << "AVERAGE" << setw(10) << "STATUS" << endl;
    cout << string(30 + numTerms * 10 + 20, '-') << endl;

    for (size_t i = 0; i < subjects.size(); ++i)
    {
        cout << left << setw(30) << subjects[i].getName();
        for (int term = 0; term < numTerms; ++term)
        {
            Grade grade = subjects[i].getGrade(term);
            if (grade.getIsSet())
            {
                cout << setw(10) << fixed << setprecision(2) << grade.getValue();
            }
            else
            {
                cout << setw(10) << "N/A";
            }
        }

        double avg = subjects[i].calculateAverage();
        string status = subjects[i].getOverallStatus();

        if (status != "N/A") 
        {
            cout << setw(10) << fixed << setprecision(2) << avg;
        }
        else 
        {
            cout << setw(10) << "N/A";
        }
        cout << setw(10) << status << endl;
    }

    cout << string(30 + numTerms * 10 + 20, '-') << endl;
    cout << left << setw(30) << "TERM AVERAGE";

    double semesterTotalAvg = 0.0;
    int validTermCount = 0;

    for (int term = 0; term < numTerms; ++term)
    {
        double termAvg = calculateTermAverage(term);
        string termStatus = getTermStatus(term);

        if (termStatus != "N/A") 
        {
            cout << setw(10) << fixed << setprecision(2) << termAvg;
            semesterTotalAvg += termAvg;
            validTermCount++;
        }
        else 
        {
            cout << setw(10) << "N/A";
        }
    }


    double semesterAvg = (validTermCount > 0) ? (semesterTotalAvg / validTermCount) : 0.0;
    string semesterStatus = "N/A";
    if (validTermCount > 0) 
    {
        semesterStatus = (semesterAvg >= PASSING_MARK) ? "Passed" : "Failed";
    }


    if (semesterStatus != "N/A") 
    {
        cout << setw(10) << fixed << setprecision(2) << semesterAvg;
    }
    else 
    {
        cout << setw(10) << "N/A";
    }
    cout << setw(10) << semesterStatus;
    cout << endl << endl;

    pauseScreen();
}

bool Semester::saveToFile(const string& filename) const
{
    ofstream outFile(filename.c_str());
    if (!outFile)
    {
        cerr << "Error: Could not open file for writing: " << filename << endl;
        return false;
    }

    outFile << name << endl;
    outFile << schoolYear << endl;
    outFile << numTerms << endl;
    outFile << subjects.size() << endl;

    for (size_t i = 0; i < subjects.size(); ++i)
    {
        outFile << subjects[i].getName() << endl;

        for (int term = 0; term < numTerms; ++term)
        {
            Grade grade = subjects[i].getGrade(term);
            outFile << grade.getIsSet() << " ";
            outFile << grade.getValue() << endl;
        }
    }

    if (outFile.fail()) 
    {
        cerr << "Error: Failed to write all data to file: " << filename << endl;
        outFile.close();
        return false;
    }

    outFile.close();
    return !outFile.fail();
}

Semester Semester::loadFromFile(const string& filename)
{
    ifstream inFile(filename.c_str());
    if (!inFile)
    {
        throw runtime_error("File not found or cannot be opened: " + filename);
    }

    string name, schoolYear, subjectName;
    int numTerms = 0;
    size_t subjectCount = 0;

    if (!getline(inFile, name)) {
        inFile.close();
        throw runtime_error("Error: Could not read semester name from: " + filename);
    }
    if (!getline(inFile, schoolYear)) {
        inFile.close();
        throw runtime_error("Error: Could not read school year from: " + filename);
    }
    if (!(inFile >> numTerms)) {
        inFile.close();
        throw runtime_error("Error: Could not read number of terms from: " + filename);
    }
    if (!(inFile >> subjectCount)) {
        inFile.close();
        throw runtime_error("Error: Could not read subject count from: " + filename);
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');


    if (numTerms < 0) {
        inFile.close();
        throw runtime_error("Error: Invalid number of terms in file: " + filename);
    }

    Semester semester(name, schoolYear, numTerms);

    for (size_t i = 0; i < subjectCount; ++i)
    {
        if (!getline(inFile, subjectName))
        {
            inFile.close();
            throw runtime_error("Error: Unexpected end of file while reading subject name in: " + filename);
        }

        semester.addSubject(subjectName);

        for (int term = 0; term < numTerms; ++term)
        {
            bool isSet = false;
            double value = 0.0;

            if (!(inFile >> isSet >> value))
            {
                inFile.close();
                throw runtime_error("Error: Invalid grade data format for subject '" + subjectName + "', term " + to_string(term + 1) + " in: " + filename);
            }
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');

            if (isSet)
            {
                try
                {
                    semester.getSubject(static_cast<int>(i)).setGrade(term, value);
                }
                catch (const out_of_range& oor)
                {
                    inFile.close();
                    throw runtime_error("Error: Subject index mismatch during load for subject '" + subjectName + "': " + string(oor.what()));
                }
            }
        }
    }

    if (inFile.fail() && !inFile.eof())
    {
        inFile.close();
        throw runtime_error("Error: Unknown error reading file after processing subjects in: " + filename);
    }

    inFile.close();
    return semester;
}