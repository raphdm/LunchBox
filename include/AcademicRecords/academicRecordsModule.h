#ifndef ACADEMICRECORDSMODULE_H
#define ACADEMICRECORDSMODULE_H

#include <string>
#include "../FeatureModule.h" 
#include "StudentProfile.h"  


using namespace std; 

class AcademicRecordsModule : public FeatureModule
{
public:
    string getName() const override;
    void run(User& user) override;
};

#endif