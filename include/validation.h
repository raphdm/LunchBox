#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <cctype>

using namespace std;

bool isValidPassword(const string& password);
bool isValidStudentID(const string& id);
bool isValidName(const string& name);
bool containsOnlyDigits(const string& str);

bool isValidSchoolYear(const string& year);
bool isValidSubjectName(const string& name);

#endif