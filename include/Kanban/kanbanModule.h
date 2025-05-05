#ifndef KANBANMODULE_H
#define KANBANMODULE_H

#include <string>
#include "../FeatureModule.h"
#include "../auth.h" 

using namespace std;

class KanbanModule : public FeatureModule
{
public:
    string getName() const override;
    void run(User& user) override;
};

#endif