#ifndef GRADE_STORAGE_H
#define GRADE_STORAGE_H

#include <string>
#include <vector>
#include "semester.h"

using namespace std;

namespace GradeStorage
{
    namespace Detail
    {
        bool createGradeDataDirectories(const string& dataDirectoryPrefix, const string& username);
        bool writeIndexFile(const string& filePath, size_t count);
        bool writeSemesterFile(const string& filePath, const Semester& semester);
        bool readIndexFile(const string& filePath, size_t& count);
        Semester readSemesterFile(const string& filePath);
        void cleanupOldSemesterFiles(const string& userDirPath, size_t currentCount);
    }
}

#endif 