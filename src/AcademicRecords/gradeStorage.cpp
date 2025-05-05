#include "../../include/AcademicRecords/gradeStorage.h"
#include <fstream>
#include <iostream>
#include <direct.h>
#include <cerrno>
#include <stdexcept>
#include <cstdio>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


using namespace std;

namespace GradeStorage
{
    namespace Detail
    {
        bool createGradeDataDirectories(const string& dataDirectoryPrefix, const string& username)
        {
            _mkdir("SavedData");

            string baseGradePath = dataDirectoryPrefix;
            string userDirPath = dataDirectoryPrefix + "/" + username;

            if (_mkdir(baseGradePath.c_str()) != 0 && errno != EEXIST)
            {
                cerr << "Error: Could not create base grade data directory: " << baseGradePath << endl;
                return false;
            }

            if (_mkdir(userDirPath.c_str()) != 0 && errno != EEXIST)
            {
                cerr << "Error: Could not create user grade data directory: " << userDirPath << endl;
                return false;
            }
            return true;
        }

        bool writeIndexFile(const string& filePath, size_t count)
        {
            ofstream indexFile(filePath.c_str());
            if (!indexFile)
            {
                cerr << "Error: Could not open index file for writing: " << filePath << endl;
                return false;
            }
            indexFile << count << endl;
            bool success = !indexFile.fail();
            indexFile.close();
            if (!success)
            {
                cerr << "Error: Failed writing to index file: " << filePath << endl;
            }
            return success;
        }

        bool writeSemesterFile(const string& filePath, const Semester& semester)
        {
            if (!semester.saveToFile(filePath))
            {
                cerr << "Error: Failed to save semester to file: " << filePath << endl;
                return false;
            }
            return true;
        }

        bool readIndexFile(const string& filePath, size_t& count)
        {
            ifstream indexFile(filePath.c_str());
            if (!indexFile)
            {
                return false;
            }
            indexFile >> count;
            bool success = !indexFile.fail() && !indexFile.bad();
            indexFile.close();
            if (!success)
            {
                cerr << "Error: Could not read semester count from index file: " << filePath << endl;
                count = 0;
            }
            return success;
        }

        Semester readSemesterFile(const string& filePath)
        {
            return Semester::loadFromFile(filePath);
        }

        void cleanupOldSemesterFiles(const string& userDirPath, size_t currentCount)
        {
            for (size_t i = currentCount; ; ++i) {
                string oldSemesterFilename = userDirPath + "/GradeRecords_" + to_string(i) + ".txt";
                ifstream checkFile(oldSemesterFilename.c_str());
                if (!checkFile) {
                    checkFile.close();
                    break;
                }
                checkFile.close();
                if (remove(oldSemesterFilename.c_str()) != 0 && errno != ENOENT) {
                    cerr << "Warning: Could not delete old semester file: " << oldSemesterFilename << " (errno: " << errno << ")" << endl;
                }
            }
        }

    }
}