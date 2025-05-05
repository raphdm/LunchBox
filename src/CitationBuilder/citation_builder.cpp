#include "../../include/CitationBuilder/citation_builder.h"
#include "../../include/CitationBuilder/citation_utils.h"
#include "../../include/utils.h"
#include <iostream>
#include <limits>
#include <memory>

using namespace std;

string CitationBuilder::getName() const
{
    return "Citation Builder";
}

void CitationBuilder::run(User& user) 
{
    bool exitModule = false;

    citationHistory.clear();
    loadCitationHistory(user.getUsername());

    while (!exitModule) 
    {
        CitationUtils::clearConsoleScreen();
        CitationUtils::printCitationBuilderTitle();

        vector<string> mainMenuOptions = 
        {
            "Create a New Citation",
            "View Citation History",
            "Manage Citation History", 
            "Back to Main Menu"
        };

        int choice = getMenuChoice(mainMenuOptions, "Please select an option: ");

        switch (choice) {
        case 1:
            createNewCitation();
            break;
        case 2:
            viewCitationHistory();
            break;
        case 3:
            manageCitationHistory(); 
            break;
        case 4:
            exitModule = true;
            break;
        default:
            CitationUtils::clearConsoleScreen();
            CitationUtils::printCitationBuilderTitle();
            cout << "Invalid option. Please try again.\n";
            CitationUtils::pauseConsole();
            break;
        }
    }

    saveCitationHistory(user.getUsername());

    citationHistory.clear();

    cout << "\nReturning to Main Menu...\n";
}

bool CitationBuilder::createNewCitation() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Create a New Citation ---\n";

    vector<string> formatOptions = 
    {
        "APA",
        "MLA",
        "Chicago",
        "Back to Citation Builder Menu"
    };

    int formatChoice = getMenuChoice(formatOptions, "Select citation format: ");

    if (formatChoice == formatOptions.size()) 
    {
        return true;
    }

    unique_ptr<CitationFormat> format(createCitationFormat(formatChoice));

    if (!format) 
    {
        CitationUtils::clearConsoleScreen();
        CitationUtils::printCitationBuilderTitle();
        cout << "Invalid format selection.\n";
        CitationUtils::pauseConsole();
        return false;
    }

    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Create a New Citation ---\n";
    cout << "Format: " << formatOptions[formatChoice - 1] << endl;

    vector<string> sourceOptions = 
    {
        "Book",
        "Article",
        "Website",
        "Research Paper",
        "Back to Citation Builder Menu"
    };

    int sourceTypeChoice = getMenuChoice(sourceOptions, "Select source type: ");

    if (sourceTypeChoice == sourceOptions.size()) 
    {
        return true;
    }

    unique_ptr<SourceType> sourceType(createSourceType(sourceTypeChoice, format.get()));

    if (!sourceType) 
    {
        CitationUtils::clearConsoleScreen();
        CitationUtils::printCitationBuilderTitle();
        cout << "Invalid source type selection.\n";
        CitationUtils::pauseConsole();
        return false;
    }

    try 
    {
        sourceType->collectInfo();

        CitationUtils::clearConsoleScreen();
        CitationUtils::printCitationBuilderTitle();

        string citation = sourceType->formatCitation();

        CitationUtils::printSeparator();
        cout << "Formatted Citation:\n";
        cout << citation << "\n";
        CitationUtils::printSeparator();

        addToCitationHistory(citation);
        cout << "Citation added to history." << endl;
    }
    catch (const exception& e)
    {
        cout << "\nError creating citation: " << e.what() << "\n";
    }

    CitationUtils::pauseConsole();
    return false;
}

void CitationBuilder::viewCitationHistory() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Citation History ---\n\n";

    if (citationHistory.empty()) 
    {
        cout << "No citations in history.\n";
    }
    else 
    {
        for (size_t i = 0; i < citationHistory.size(); ++i) 
        {
            cout << i + 1 << ". " << citationHistory[i] << "\n\n";
        }
    }
    CitationUtils::pauseConsole();
}

void CitationBuilder::manageCitationHistory() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Manage Citation History ---\n\n";

    if (citationHistory.empty()) 
    {
        cout << "Citation history is empty.\n";
        CitationUtils::pauseConsole();
        return;
    }

    vector<string> historyOptions = 
    {
        "Delete All Citation History",
        "Delete Specific Citation",
        "Back to Main Menu"
    };

    int choice = getMenuChoice(historyOptions, "Please select an option: ");

    switch (choice) 
    {
    case 1:
        deleteAllCitationHistory();
        break;
    case 2:
        deleteSpecificCitation();
        break;
    case 3:
        return;
    default:
        CitationUtils::clearConsoleScreen();
        CitationUtils::printCitationBuilderTitle();
        cout << "Invalid option. Please try again.\n";
        CitationUtils::pauseConsole();
        break;
    }
}

void CitationBuilder::deleteAllCitationHistory() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Delete All Citation History ---\n\n";

    if (citationHistory.empty()) 
    {
        cout << "Citation history is already empty.\n";
    }
    else
    {
        if (getYesNoInput("Are you sure you want to delete your entire citation history? This action cannot be undone.")) 
        {
            citationHistory.clear();
            cout << "\nCitation history deleted successfully.\n";
        }
        else 
        {
            cout << "\nDeletion cancelled.\n";
        }
    }
    CitationUtils::pauseConsole();
}

void CitationBuilder::deleteSpecificCitation() 
{
    CitationUtils::clearConsoleScreen();
    CitationUtils::printCitationBuilderTitle();
    cout << "\n--- Delete Specific Citation ---\n\n";

    if (citationHistory.empty()) 
    {
        cout << "No citations in history to delete.\n";
        CitationUtils::pauseConsole();
        return;
    }

    cout << "Select a citation to delete:\n\n";
    for (size_t i = 0; i < citationHistory.size(); ++i) 
    {
        cout << i + 1 << ". " << citationHistory[i] << "\n\n";
    }

    int selection = 0;
    bool validSelection = false;

    while (!validSelection) 
    {
        cout << "Enter the number of the citation to delete (or 0 to cancel): ";
        string input;
        getline(cin, input);

        try 
        {
            size_t processedChars;
            selection = stoi(input, &processedChars);

            if (processedChars == input.length() && selection >= 0 && selection <= static_cast<int>(citationHistory.size())) 
            {
                validSelection = true;
            }
            else {
                cout << "\nInvalid input. Please enter a number between 0 and " << citationHistory.size() << ".\n";
            }
        }
        catch (const invalid_argument&) 
        {
            cout << "\nInvalid input. Please enter a number.\n";
        }
        catch (const out_of_range&) 
        {
            cout << "\nInput is out of valid range.\n";
        }
    }

    if (selection == 0) 
    {
        cout << "\nDeletion cancelled.\n";
    }
    else {
        if (getYesNoInput("Are you sure you want to delete this citation? This action cannot be undone.")) 
        {
            citationHistory.erase(citationHistory.begin() + selection - 1);
            cout << "\nCitation deleted successfully.\n";
        }
        else 
        {
            cout << "\nDeletion cancelled.\n";
        }
    }

    CitationUtils::pauseConsole();
}

int CitationBuilder::getMenuChoice(const vector<string>& options, const string& prompt)
{
    int choice = 0;
    bool validChoice = false;

    while (!validChoice) 
    {
        CitationUtils::clearConsoleScreen();
        CitationUtils::printCitationBuilderTitle();

        cout << endl;
        for (size_t i = 0; i < options.size(); ++i) 
        {
            cout << i + 1 << ". " << options[i] << endl;
        }
        cout << endl;

        cout << prompt;
        string input;
        getline(cin, input);

        try 
        {
            size_t processedChars;
            choice = stoi(input, &processedChars);

            if (processedChars == input.length() && choice >= 1 && choice <= static_cast<int>(options.size())) 
            {
                validChoice = true;
            }
            else 
            {
                cout << "\nInvalid input. Please enter a number between 1 and " << options.size() << "." << endl;
                CitationUtils::pauseConsole();
            }
        }
        catch (const invalid_argument&) 
        {
            cout << "\nInvalid input. Please enter a number." << endl;
            CitationUtils::pauseConsole();
        }
        catch (const out_of_range&) 
        {
            cout << "\nInput year is out of valid range." << endl;
            CitationUtils::pauseConsole();
        }
    }

    return choice;
}

unique_ptr<CitationFormat> CitationBuilder::createCitationFormat(int formatChoice) 
{
    switch (formatChoice) 
    {
    case 1:
        return make_unique<APAFormat>();
    case 2:
        return make_unique<MLAFormat>();
    case 3:
        return make_unique<ChicagoFormat>();
    default:
        return nullptr;
    }
}

unique_ptr<SourceType> CitationBuilder::createSourceType(int sourceTypeChoice, CitationFormat* format) 
{
    switch (sourceTypeChoice)
    {
    case 1:
        return make_unique<Book>(format);
    case 2:
        return make_unique<Article>(format);
    case 3:
        return make_unique<Website>(format);
    case 4:
        return make_unique<ResearchPaper>(format);
    default:
        return nullptr;
    }
}

void CitationBuilder::addToCitationHistory(const string& citation) 
{
    citationHistory.push_back(citation);
}

vector<string> CitationBuilder::getCitationHistory() const 
{
    return citationHistory;
}

void CitationBuilder::saveCitationHistory(const string& username) const 
{
    if (!CitationUtils::saveCitationHistoryToFile(citationHistory, username)) 
    {
        cerr << "Error: Failed to save citation history for user " << username << "." << endl;
    }
}

void CitationBuilder::loadCitationHistory(const string& username) 
{
    if (!CitationUtils::loadCitationHistoryFromFile(citationHistory, username)) 
    {
        cout << "No existing citation history found for user " << username << "." << endl;
    }
}

