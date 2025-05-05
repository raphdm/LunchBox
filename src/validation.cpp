#include "../include/validation.h"
#include <stdexcept>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

bool isValidPassword(const string& password)
{
    return password.length() >= 8 && password.length() <= 16;
}

bool isValidStudentID(const string& id)
{
    return !id.empty() && id.length() >= 5;
}

bool isValidName(const string& name)
{
    if (name.empty())
    {
        return false;
    }
    for (size_t i = 0; i < name.length(); i++)
    {

        if (isdigit(name[i]) && !isspace(name[i]) && name[i] != '-')
        {
            return false;
        }
    }

    bool nonSpaceFound = false;
    for (size_t i = 0; i < name.length(); i++) 
    {
        if (!isspace(name[i]))
        {
            nonSpaceFound = true;
            break;
        }
    }
    return nonSpaceFound;
}

bool containsOnlyDigits(const string& str)
{
    if (str.empty())
    {
        return false;
    }
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool isValidSchoolYear(const string& year)
{

    if (year.length() != 9 || year[4] != '-')
    {
        return false;
    }

    string firstPart = year.substr(0, 4);
    string secondPart = year.substr(5, 4);

    if (!containsOnlyDigits(firstPart) || !containsOnlyDigits(secondPart)) 
    {
        return false;
    }

    try {
        int firstYear = stoi(firstPart);
        int secondYear = stoi(secondPart);


        return (secondYear == firstYear + 1) && (firstYear >= 1900 && secondYear <= 2100);
    }
    catch (const out_of_range&) 
    {
        return false;
    }
    catch (...) 
    {

        return false;
    }
}

bool isValidSubjectName(const string& name)
{
    if (name.empty()) 
    {
        return false;
    }

    for (size_t i = 0; i < name.length(); ++i)
    {
        if (!isspace(static_cast<unsigned char>(name[i])))
        {
            return true;
        }
    }
    return false;
}