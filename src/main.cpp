#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../include/auth.h"
#include "../include/utils.h"
#include "../include/FeatureModule.h"
#include "../include/AcademicRecords/academicRecordsModule.h"
#include "../include/Kanban/kanbanModule.h"
#include "../include/CitationBuilder/citation_builder.h"
#include "../include/NoteTaking/noteTakingModule.h"

using namespace std;

void displayMainMenu(User& user, const vector<FeatureModule*>& modules);

int main()
{
    User currentUser;

    if (!displayLoginMenu(currentUser))
    {
        return 0;
    }

    vector<FeatureModule*> featureModules;
    featureModules.push_back(new AcademicRecordsModule());
    featureModules.push_back(new KanbanModule());
    featureModules.push_back(new CitationBuilder());
    featureModules.push_back(new NoteTakingModule());


    displayMainMenu(currentUser, featureModules);

    for (size_t i = 0; i < featureModules.size(); ++i)
    {
        delete featureModules[i];
    }
    featureModules.clear();

    return 0;
}