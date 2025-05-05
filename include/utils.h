#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <limits>
#include "auth.h"
#include "FeatureModule.h"

using namespace std;

void printUserAuthMenu();
void printLoginTitle();
void printRegisterTitle();
void displayMainMenu(User& user, const vector<FeatureModule*>& modules);
void printAcademicRecordsSubMenu();
void printAddOrEditInfoTitle();
void printDeleteProfileTitle();
void printViewMyInfoTitle();
void pauseScreen();
void clearInputStream();

void clearScreen();
void displayHeader(const string& title);
void displayMenuOptions(const vector<string>& options);

int getIntInput(const string& prompt, int min = numeric_limits<int>::min(), int max = numeric_limits<int>::max());
double getDoubleInput(const string& prompt, double min = numeric_limits<double>::lowest(), double max = numeric_limits<double>::max());
string getStringInput(const string& prompt, bool allowEmpty = false, int minLength = 0, int maxLength = 100);
bool getYesNoInput(const string& prompt);

#endif