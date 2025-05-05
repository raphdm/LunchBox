#ifndef GRADE_TRACKER_UI_H
#define GRADE_TRACKER_UI_H

#include "semester.h"
#include "../../include/auth.h"


using namespace std;

void manageGradeTracking(User& user);
void manageSemester(Semester& semester);
void editSubject(Subject& subject);
void editSemesterDetails(Semester& semester);


#endif 