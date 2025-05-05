#ifndef CITATION_FORMATS_H
#define CITATION_FORMATS_H

#include <string>
#include <map>

using namespace std;

class CitationFormat 
{
protected:
    map<string, string> sourceDetails;

public:
    CitationFormat() {}
    virtual ~CitationFormat() {}

    virtual string formatCitation(const string& sourceType) const = 0;

    void setDetail(const string& key, const string& value);
    map<string, string> getDetails() const;
    string getDetailSafe(const string& key) const;
};

class APAFormat : public CitationFormat
{
public:
    APAFormat() : CitationFormat() {}
    string formatCitation(const string& sourceType) const override;
};

class MLAFormat : public CitationFormat 
{
public:
    MLAFormat() : CitationFormat() {}
    string formatCitation(const string& sourceType) const override;
};

class ChicagoFormat : public CitationFormat 
{
public:
    ChicagoFormat() : CitationFormat() {}
    string formatCitation(const string& sourceType) const override;
};

#endif