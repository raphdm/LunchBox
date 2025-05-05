#ifndef NOTETAKINGMODULE_H
#define NOTETAKINGMODULE_H

#include <string>
#include "../FeatureModule.h"
#include "../auth.h"

using namespace std;

class NoteTakingModule : public FeatureModule
{
public:
    string getName() const override;
    void run(User& user) override;
};

#endif