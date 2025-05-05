#include "../../include/Kanban/kanban_utils.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <regex>
#include <algorithm>
#include <locale>
#include <ctime>
#include <cstring>
#include <stdexcept> 

using namespace std;

namespace KanbanUtils
{
    void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void displayHeader()
    {
        cout << horizontalLine('=') << endl;
        cout << "|" << setw(TERMINAL_WIDTH - 2) << left << "                                          KANBAN BOARD" << "|" << endl;

        time_t currentTime = time(nullptr);
        char timeStr[26];

#ifdef _WIN32
        struct tm timeinfo;
        if (localtime_s(&timeinfo, &currentTime) == 0) {
            asctime_s(timeStr, sizeof(timeStr), &timeinfo);
        }
        else {
            strncpy_s(timeStr, "Error getting time", sizeof(timeStr));
            timeStr[sizeof(timeStr) - 1] = '\0';
        }
#else
        struct tm timeinfo;
        if (localtime_r(&currentTime, &timeinfo) != nullptr) {
            if (asctime_r(&timeinfo, timeStr) == nullptr) {
                strncpy(timeStr, "Error formatting time", sizeof(timeStr));
                timeStr[sizeof(timeStr) - 1] = '\0';
            }
        }
        else {
            strncpy(timeStr, "Error getting time", sizeof(timeStr));
            timeStr[sizeof(timeStr) - 1] = '\0';
        }
#endif

        size_t len = strlen(timeStr);
        if (len > 0 && timeStr[len - 1] == '\n')
        {
            timeStr[len - 1] = '\0';
        }

        cout << "|" << setw(TERMINAL_WIDTH - 2) << right << timeStr << "|" << endl;
        cout << horizontalLine('=') << endl;
    }

    void displayFooter()
    {
        cout << horizontalLine('-') << endl;
        cout << "Kanban Board Organizer - Manage your tasks efficiently" << endl;
    }

    bool isValidDate(const string& date)
    {
        regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");
        if (!regex_match(date, datePattern))
        {
            return false;
        }

        try {
            int year = stoi(date.substr(0, 4));
            int month = stoi(date.substr(5, 2));
            int day = stoi(date.substr(8, 2));

            if (month < 1 || month > 12 || day < 1)
            {
                return false;
            }

            if (day > 31) return false;
            if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;

            if (month == 2) {
                bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                if (isLeap && day > 29) return false;
                if (!isLeap && day > 28) return false;
            }

            return true;

        }
        catch (const std::invalid_argument&) {
            return false;
        }
        catch (const std::out_of_range&) {
            return false;
        }
    }

    string getFormattedDate(const string& date)
    {
        if (!isValidDate(date))
        {
            return date;
        }

        try {
            const string monthNames[] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

            int year = stoi(date.substr(0, 4));
            int month = stoi(date.substr(5, 2));
            int day = stoi(date.substr(8, 2));

            stringstream ss;
            ss << setfill('0') << setw(2) << day << " "
                << monthNames[month] << " " << year;

            return ss.str();
        }
        catch (const std::invalid_argument&) {
            return date;
        }
        catch (const std::out_of_range&) {
            return date;
        }
    }

    string colorByStatus(const string& status)
    {
        if (status == "To Do")
        {
            return "\033[1;31m";
        }
        else if (status == "In Progress")
        {
            return "\033[1;33m";
        }
        else if (status == "Done")
        {
            return "\033[1;32m";
        }
        else {
            return "\033[0m";
        }
    }

    string resetColor()
    {
        return "\033[0m";
    }

    void displayTasksInColumns(const vector<Task>& todoTasks,
        const vector<Task>& inProgressTasks,
        const vector<Task>& doneTasks)
    {
        cout << "\033[1;36m";
        cout << setw(COLUMN_WIDTH) << left << "TO DO";
        cout << " | ";
        cout << setw(COLUMN_WIDTH) << left << "IN PROGRESS";
        cout << " | ";
        cout << setw(COLUMN_WIDTH) << left << "DONE" << resetColor() << endl;

        cout << horizontalLine('-') << endl;

        size_t maxTasks = 0;
        if (!todoTasks.empty()) maxTasks = max(maxTasks, todoTasks.size());
        if (!inProgressTasks.empty()) maxTasks = max(maxTasks, inProgressTasks.size());
        if (!doneTasks.empty()) maxTasks = max(maxTasks, doneTasks.size());


        for (size_t i = 0; i < maxTasks; i++)
        {
            string todoContent = (i < todoTasks.size()) ?
                formatTaskForColumn(todoTasks[i]) : "";

            string inProgressContent = (i < inProgressTasks.size()) ?
                formatTaskForColumn(inProgressTasks[i]) : "";

            string doneContent = (i < doneTasks.size()) ?
                formatTaskForColumn(doneTasks[i]) : "";

            vector<string> todoLines, inProgressLines, doneLines;
            istringstream todoStream(todoContent);
            string line;
            while (getline(todoStream, line)) { todoLines.push_back(line); }
            while (todoLines.size() < 2) todoLines.push_back("");

            istringstream inProgressStream(inProgressContent);
            while (getline(inProgressStream, line)) { inProgressLines.push_back(line); }
            while (inProgressLines.size() < 2) inProgressLines.push_back("");

            istringstream doneStream(doneContent);
            while (getline(doneStream, line)) { doneLines.push_back(line); }
            while (doneLines.size() < 2) doneLines.push_back("");

            string todoColor = colorByStatus("To Do");
            string inProgressColor = colorByStatus("In Progress");
            string doneColor = colorByStatus("Done");

            cout << todoColor << setw(COLUMN_WIDTH) << left << todoLines[0] << resetColor();
            cout << " | ";
            cout << inProgressColor << setw(COLUMN_WIDTH) << left << inProgressLines[0] << resetColor();
            cout << " | ";
            cout << doneColor << setw(COLUMN_WIDTH) << left << doneLines[0] << resetColor();
            cout << endl;

            cout << todoColor << setw(COLUMN_WIDTH) << left << todoLines[1] << resetColor();
            cout << " | ";
            cout << inProgressColor << setw(COLUMN_WIDTH) << left << inProgressLines[1] << resetColor();
            cout << " | ";
            cout << doneColor << setw(COLUMN_WIDTH) << left << doneLines[1] << resetColor();
            cout << endl;

            if (i < maxTasks - 1)
            {
                cout << string(COLUMN_WIDTH, '.') << "-+-" << string(COLUMN_WIDTH, '.') << "-+-" << string(COLUMN_WIDTH, '.') << endl;
            }
            else {
                cout << horizontalLine('=') << endl;
            }
        }
        if (maxTasks == 0) {
            cout << setw(COLUMN_WIDTH) << " " << " | " << setw(COLUMN_WIDTH) << " " << " | " << setw(COLUMN_WIDTH) << " " << endl;
            cout << horizontalLine('=') << endl;
        }
    }

    string formatTaskForColumn(const Task& task)
    {
        stringstream ss;

        string title = truncateText(task.getTitle(), COLUMN_WIDTH - 6);
        string date = getFormattedDate(task.getDueDate());

        ss << "#" << task.getId() << ": " << title << endl;
        ss << "Due: " << date;

        return ss.str();
    }

    string horizontalLine(char c, int width) 
    {

        return string(TERMINAL_WIDTH, c); 
    }

    string truncateText(const string& text, int maxWidth)
    {
        if (maxWidth <= 3) return "...";
        if (static_cast<int>(text.length()) <= maxWidth)
        {
            return text;
        }

        return text.substr(0, max(0, maxWidth - 3)) + "...";
    }
}