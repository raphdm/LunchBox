#ifndef CITATION_BUILDER_H
#define CITATION_BUILDER_H

#include <string>
#include <vector>
#include <memory>
#include "../../include/auth.h"
#include "../../include/FeatureModule.h"
#include "citation_formats.h"
#include "source_types.h"
#include "citation_utils.h"

using namespace std;

class CitationBuilder : public FeatureModule
{
private:
    vector<string> citationHistory;

    int getMenuChoice(const vector<string>& options, const string& prompt);
    unique_ptr<CitationFormat> createCitationFormat(int formatChoice);
    unique_ptr<SourceType> createSourceType(int sourceTypeChoice, CitationFormat* format);

    void saveCitationHistory(const string& username) const; 
    void loadCitationHistory(const string& username); 
    void deleteAllCitationHistory();
    void deleteSpecificCitation(); 

public:
    string getName() const override;
    void run(User& user) override;
    bool createNewCitation();
    void viewCitationHistory();
    void manageCitationHistory(); 

    void addToCitationHistory(const string& citation);
    vector<string> getCitationHistory() const;
};

#endif