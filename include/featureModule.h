#ifndef FEATUREMODULE_H
#define FEATUREMODULE_H
#include <string>
#include "auth.h"

using namespace std;

class FeatureModule
{
public:
    virtual ~FeatureModule() {}
    virtual string getName() const = 0;
    virtual void run(User& user) = 0;
};

#endif 