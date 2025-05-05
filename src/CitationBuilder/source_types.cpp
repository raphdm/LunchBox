#include "../../include/CitationBuilder/source_types.h"
#include "../../include/CitationBuilder/citation_utils.h" 
#include <iostream>
#include <regex>
#include <limits>
#include <sstream>
#include <typeinfo>

using namespace std;


string SourceType::formatCitation() const 
{
    string sourceTypeName = getSourceTypeName();
    return format->formatCitation(sourceTypeName);
}

string SourceType::getSourceTypeName() const 
{
    const type_info& info = typeid(*this);
    string name = info.name();

    size_t pos = name.find_last_of("::");
    if (pos != string::npos) 
    {
        name = name.substr(pos + 1);
    }

    if (name.find("class ") == 0) 
    {
        name = name.substr(6);
    }

    pos = 0;
    while (pos < name.length() && isdigit(name[pos])) 
    {
        pos++;
    }

    if (pos > 0) {
        name = name.substr(pos);
    }

    return name;
}

string SourceType::getValidInput(const string& prompt, bool allowEmpty) 
{
    string input;
    bool valid = false;

    while (!valid) 
    {
        cout << prompt;
        getline(cin, input);

        input = CitationUtils::trimString(input);

        if (input.empty() && !allowEmpty) 
        {
            cout << "Input cannot be empty. Please try again." << endl;
        }
        else {
            valid = true;
        }
    }

    return input;
}

int SourceType::getValidYear(const string& prompt) 
{
    int year = 0;
    bool valid = false;
    const int currentYear = 2025;

    while (!valid) {
        cout << prompt;
        string input;
        getline(cin, input);

        try {
            size_t processedChars;
            year = stoi(input, &processedChars);

            if (processedChars == input.length() && year >= 1500 && year <= currentYear) 
            {
                valid = true;
            }
            else 
            {
                cout << "Please enter a valid year between 1500 and " << currentYear << "." << endl;
            }
        }
        catch (const invalid_argument&) 
        {
            cout << "Invalid input. Please enter a number." << endl;
        }
        catch (const out_of_range&) 
        {
            cout << "Input year is out of valid range." << endl;
        }
    }

    return year;
}

string SourceType::getValidPages(const string& prompt) 
{
    string pages;
    bool valid = false;
    regex pagesPattern("^[0-9]+(\\s*-\\s*[0-9]+)?$");

    while (!valid) {
        cout << prompt;
        getline(cin, pages);

        pages = CitationUtils::trimString(pages);

        if (regex_match(pages, pagesPattern)) 
        {
            valid = true;
        }
        else {
            cout << "Invalid page format. Use single page (e.g., 5) or range (e.g., 5-10)." << endl;
        }
    }

    return pages;
}

string SourceType::getValidDate(const string& prompt) 
{
    string date;
    bool valid = false;
    regex datePattern(R"(^(0?[1-9]|1[0-2])/(0?[1-9]|[12][0-9]|3[01])/[0-9]{4}$)");

    while (!valid) 
    {
        cout << prompt;
        getline(cin, date);

        if (regex_match(date, datePattern)) 
        {
            istringstream ss(date);
            int month, day, year;
            char delimiter;

            if (ss >> month >> delimiter >> day >> delimiter >> year) 
            {
                bool validDay = true;

                if (month == 2) 
                {
                    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                    if ((isLeapYear && day > 29) || (!isLeapYear && day > 28)) 
                    {
                        validDay = false;
                    }
                }
                else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) 
                {
                    validDay = false;
                }

                if (validDay) 
                {
                    valid = true;
                }
                else 
                {
                    cout << "Invalid date. Please check the number of days in the month." << endl;
                }
            }
            else 
            {
                cout << "Error parsing date. Please use MM/DD/YYYY format." << endl;
            }
        }
        else 
        {
            cout << "Invalid date format. Use MM/DD/YYYY (e.g., 04/30/2025 or 4/30/2025)." << endl;
        }
    }

    return date;
}

string SourceType::getValidURL(const string& prompt) 
{
    string url;
    bool valid = false;
    regex urlPattern(R"(^(https?://)?([\w-]+\.)+[\w-]+(:[0-9]+)?(/[\w-%./&?=]*)?$)");

    while (!valid) 
    {
        cout << prompt;
        getline(cin, url);

        if (regex_match(url, urlPattern)) 
        {
            if (url.find("http://") != 0 && url.find("https://") != 0) 
            {
                url = "https://" + url;
            }
            valid = true;
        }
        else {
            cout << "Invalid URL format. Please enter a valid URL (e.g., https://example.com)." << endl;
        }
    }

    return url;
}

void Book::collectInfo() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Book Citation Information ---\n";

    string author = getValidInput("Enter author name (Last, First M.): ");
    string title = getValidInput("Enter book title: ");
    string publisher = getValidInput("Enter publisher: ");
    int year = getValidYear("Enter year published: ");
    
    string publisherLocation;
    if (dynamic_cast<ChicagoFormat*>(format) != nullptr) 
    {
        publisherLocation = getValidInput("Enter publisher location (leave blank if unknown): ", true);
    }

    format->setDetail("author", author);
    format->setDetail("title", title);
    format->setDetail("publisher", publisher);
    format->setDetail("year", to_string(year));

    if (!publisherLocation.empty()) 
    {
        format->setDetail("publisherLocation", publisherLocation);
    }
}

void Article::collectInfo() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle(); 
    cout << "\n--- Article Citation Information ---\n";

    string author = getValidInput("Enter author name (Last, First M.): ");
    string title = getValidInput("Enter article title: ");
    string journal = getValidInput("Enter journal name: ");
    string volume = getValidInput("Enter volume number: ");
    string issue = getValidInput("Enter issue number: ");
    string pages = getValidPages("Enter page range (e.g., 125-140): ");
    int year = getValidYear("Enter publication year: ");

    format->setDetail("author", author);
    format->setDetail("title", title);
    format->setDetail("journal", journal);
    format->setDetail("volume", volume);
    format->setDetail("issue", issue);
    format->setDetail("pages", pages);
    format->setDetail("year", to_string(year));
}

void Website::collectInfo() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle(); 
    cout << "\n--- Website Citation Information ---\n";

    string author = getValidInput("Enter author/organization name: ");
    string title = getValidInput("Enter page title: ");
    string websiteName = getValidInput("Enter website name: ");
    string url = getValidURL("Enter URL: ");
    string dateAccessed = getValidDate("Enter date accessed (MM/DD/YYYY): ");
    string yearPublished = getValidInput("Enter year published/updated (leave blank if unknown): ", true);

    format->setDetail("author", author);
    format->setDetail("title", title);
    format->setDetail("websiteName", websiteName);
    format->setDetail("url", url);
    format->setDetail("dateAccessed", dateAccessed);
    format->setDetail("year", yearPublished);
}

void ResearchPaper::collectInfo() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle(); // Update call
    cout << "\n--- Research Paper Citation Information ---\n";

    string author = getValidInput("Enter author name (Last, First M.): ");
    string title = getValidInput("Enter paper title: ");
    string institution = getValidInput("Enter institution/university: ");
    int year = getValidYear("Enter publication year: ");

    format->setDetail("author", author);
    format->setDetail("title", title);
    format->setDetail("institution", institution);
    format->setDetail("year", to_string(year));
}