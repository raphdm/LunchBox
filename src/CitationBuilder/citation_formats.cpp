#include "../../include/CitationBuilder/citation_formats.h"
#include <sstream>

using namespace std;

void CitationFormat::setDetail(const string& key, const string& value) 
{
    sourceDetails[key] = value;
}

map<string, string> CitationFormat::getDetails() const 
{
    return sourceDetails;
}

string CitationFormat::getDetailSafe(const string& key) const 
{
    auto it = sourceDetails.find(key);
    if (it != sourceDetails.end()) {
        return it->second;
    }
    return "";
}

string APAFormat::formatCitation(const string& sourceType) const 
{
    stringstream citation;

    if (sourceType == "Book") 
    {
        citation << getDetailSafe("author") << " ("
            << getDetailSafe("year") << "). "
            << getDetailSafe("title") << ". "
            << getDetailSafe("publisher") << ".";
    }
    else if (sourceType == "Article") {
        citation << getDetailSafe("author") << " ("
            << getDetailSafe("year") << "). "
            << getDetailSafe("title") << ". "
            << getDetailSafe("journal") << ", "
            << getDetailSafe("volume") << "("
            << getDetailSafe("issue") << "), "
            << getDetailSafe("pages") << ".";
    }
    else if (sourceType == "Website") {
        citation << getDetailSafe("author") << " ";

        if (!getDetailSafe("year").empty()) {
            citation << "(" << getDetailSafe("year") << "). ";
        }

        citation << getDetailSafe("title") << ". "
            << getDetailSafe("websiteName") << ". "
            << "Retrieved " << getDetailSafe("dateAccessed")
            << ", from " << getDetailSafe("url") << ".";
    }
    else if (sourceType == "ResearchPaper") {
        citation << getDetailSafe("author") << " ("
            << getDetailSafe("year") << "). "
            << getDetailSafe("title") << ". "
            << getDetailSafe("institution") << ".";
    }

    return citation.str();
}

string MLAFormat::formatCitation(const string& sourceType) const 
{
    stringstream citation;

    if (sourceType == "Book")
    {
        citation << getDetailSafe("author") << ". "
            << getDetailSafe("title") << ". "
            << getDetailSafe("publisher") << ", "
            << getDetailSafe("year") << ".";
    }
    else if (sourceType == "Article") {
        citation << getDetailSafe("author") << ". \""
            << getDetailSafe("title") << ".\" "
            << getDetailSafe("journal") << ", vol. "
            << getDetailSafe("volume") << ", no. "
            << getDetailSafe("issue") << ", "
            << getDetailSafe("year") << ", pp. "
            << getDetailSafe("pages") << ".";
    }
    else if (sourceType == "Website") 
    {
        citation << getDetailSafe("author") << ". \""
            << getDetailSafe("title") << ".\" "
            << getDetailSafe("websiteName") << ", ";

        if (!getDetailSafe("year").empty()) 
        {
            citation << getDetailSafe("year") << ", ";
        }

        citation << getDetailSafe("url") << ". Accessed "
            << getDetailSafe("dateAccessed") << ".";
    }
    else if (sourceType == "ResearchPaper") 
    {
        citation << getDetailSafe("author") << ". \""
            << getDetailSafe("title") << ".\" "
            << getDetailSafe("institution") << ", "
            << getDetailSafe("year") << ".";
    }

    return citation.str();
}

string ChicagoFormat::formatCitation(const string& sourceType) const 
{
    stringstream citation;

    if (sourceType == "Book")
    {
        citation << getDetailSafe("author") << ". "
            << getDetailSafe("title") << ". ";

        if (!getDetailSafe("publisherLocation").empty()) 
        {
            citation << getDetailSafe("publisherLocation") << ": ";
        }

        citation << getDetailSafe("publisher") << ", "
            << getDetailSafe("year") << ".";
    }
    else if (sourceType == "Article") 
    {
        citation << getDetailSafe("author") << ". \""
            << getDetailSafe("title") << ".\" "
            << getDetailSafe("journal") << " "
            << getDetailSafe("volume") << ", no. "
            << getDetailSafe("issue") << " ("
            << getDetailSafe("year") << "): "
            << getDetailSafe("pages") << ".";
    }
    else if (sourceType == "Website")
    {
        citation << getDetailSafe("author") << ". \""
            << getDetailSafe("title") << ".\" "
            << getDetailSafe("websiteName") << ". ";

        if (!getDetailSafe("year").empty()) 
        {
            citation << "Published/Modified " << getDetailSafe("year") << ". ";
        }

        citation << getDetailSafe("url") << " (accessed "
            << getDetailSafe("dateAccessed") << ").";
    }
    else if (sourceType == "ResearchPaper") 
    {
        citation << getDetailSafe("author") << ". \""
            << getDetailSafe("title") << ".\" "
            << "Research Paper, " << getDetailSafe("institution")
            << ", " << getDetailSafe("year") << ".";
    }

    return citation.str();
}