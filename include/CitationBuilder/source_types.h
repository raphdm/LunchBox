#ifndef SOURCE_TYPES_H
#define SOURCE_TYPES_H

#include <string>
#include <vector>
#include <typeinfo>
#include "citation_formats.h"

using namespace std;

class SourceType 
{
protected:
    CitationFormat* format;

public:
    SourceType(CitationFormat* fmt) : format(fmt) {}
    virtual ~SourceType() {}

    virtual void collectInfo() = 0;

    string formatCitation() const;
    string getSourceTypeName() const;

    string getValidInput(const string& prompt, bool allowEmpty = false);
    int getValidYear(const string& prompt);
    string getValidPages(const string& prompt);
    string getValidDate(const string& prompt);
    string getValidURL(const string& prompt);
};

class Book : public SourceType 
{
public:
    Book(CitationFormat* fmt) : SourceType(fmt) {}
    void collectInfo() override;
};

class Article : public SourceType 
{
public:
    Article(CitationFormat* fmt) : SourceType(fmt) {}
    void collectInfo() override;
};

class Website : public SourceType 
{
public:
    Website(CitationFormat* fmt) : SourceType(fmt) {}
    void collectInfo() override;
};

class ResearchPaper : public SourceType 
{
public:
    ResearchPaper(CitationFormat* fmt) : SourceType(fmt) {}
    void collectInfo() override;
};

#endif