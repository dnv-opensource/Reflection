//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SymbolReplacer.h"
#include <regex>

using namespace std;

namespace DNVS { namespace MoFa { namespace Scripting {

SymbolReplacer::SymbolReplacer(string symbol)
    : m_symbol(symbol)
    , m_previousGotStuckInText(empty)
{

}

void SymbolReplacer::AddHandler(const string& postfix, const function<string(long)>& callback)
{
    m_handlers.emplace_back(std::make_shared<function<string(long)>>(callback));
    m_identifiers.emplace_back(postfix);
}

string& SymbolReplacer::Replace(const string& text)
{
    list<pair<size_t, size_t>> intervals = FindSearchIntervals(text);
    m_replacement = "";
    size_t currentPos = 0;
    for (const auto& pair : intervals)
    {
        if (currentPos != pair.first)
        {
            m_replacement += text.substr(currentPos, pair.first-currentPos);
        }
        m_replacement += ReplaceTextInInterval(text, pair);
        currentPos = pair.second;
    }
    if (currentPos != text.size())
        m_replacement += text.substr(currentPos, text.size()-currentPos);

    return m_replacement;
}

std::string& SymbolReplacer::Replace(const char* text, size_t length)
{
    return Replace(string(text, length));
}

std::string SymbolReplacer::GetSymbol() const
{
    return m_symbol;
}

void SymbolReplacer::Reset()
{
    m_previousGotStuckInText = empty;
}

// Symbols inside JS text should not be replaced, so the intervals finds everything outside JS strings
list<pair<size_t, size_t>> SymbolReplacer::FindSearchIntervals(const string& text)
{
    list<pair<size_t, size_t>> intervals;
    bool startFound = false;
    bool endFound = false;
    size_t currentPos = 0;
    while (true)
    {
        size_t startSingle, startDouble;
        tie(startSingle, startDouble) = GetQuotationPos(text, currentPos);
        size_t start = min(startSingle, startDouble);
        if (start != string::npos)
        {
            startFound = true;
            size_t end = findEndQuotation(text, start, startSingle < startDouble);
            if (currentPos != start)
                intervals.emplace_back(std::make_pair(currentPos, start));
            if (end != string::npos)
            {
                endFound = true;
                currentPos = end + 1;
                continue;
            }
            else
            {
                // start found, but not end
                endFound = false;
                m_previousGotStuckInText = (startSingle < startDouble) ? singleQuote : doubleQuote;
            }
        }
        break; // no start and end found, then we are done
    }
    if (startFound && endFound && currentPos != text.size())
        intervals.emplace_back(std::make_pair(currentPos, text.size()));
    if(!startFound && !endFound) // algorithm does not handle string with no quotes, so add the entire string interval here
        intervals.emplace_back(std::make_pair(size_t(0), text.size()));

    return intervals;
}

pair<size_t, size_t> SymbolReplacer::GetQuotationPos(const string& text, size_t currentPos)
{
    size_t startSingle = string::npos;
    size_t startDouble = string::npos;
    if (m_previousGotStuckInText != empty)
    {
        if (m_previousGotStuckInText == singleQuote)
            startSingle = 0;
        else
            startDouble = 0;
        m_previousGotStuckInText = empty;
    }
    else
    {
        startSingle = text.find('\'', currentPos);
        startDouble = text.find('"', currentPos);
    }

    return make_pair(startSingle, startDouble);
}

size_t SymbolReplacer::findEndQuotation(const std::string& text, size_t start, bool checkSingleQuotationMark)
{
    bool escapeFound;
    size_t end;
    size_t searchStart = start + 1;
    do
    {
        escapeFound = false;
        if (checkSingleQuotationMark)
            end = text.find('\'', searchStart);
        else
            end = text.find('"', searchStart);

        // check for escape character
        if (end != string::npos && text.at(end - 1) == '\\')
        {
            searchStart = end + 1;
            escapeFound = true;
        }
    } while (escapeFound);
    return end;
}

bool IsMatchingSymbol(const std::string& text, const std::string& id)
{
    if (id.empty())
    {
        if (!text.empty() && !isalpha(text.at(0)))
            return true;
        else
            return false;
    }
    return text.substr(0, id.size()) == id;
}
std::string SymbolReplacer::ReplaceTextInInterval(const std::string& text, std::pair<size_t, size_t> interval)
{
    string textInInterval = text.substr(interval.first, interval.second-interval.first);
    bool found = true;
    size_t currentPos = 0;
    size_t prevPos;
    string replacedText = "";
    while (found)
    {
        prevPos = currentPos;
        currentPos = textInInterval.find(m_symbol, currentPos);
        if (currentPos == string::npos)
        {
            found = false;
        }
        else {
            string nextStr = textInInterval.substr(currentPos + m_symbol.size()); // filter away symbol
            bool foundId = false;
            for (int i = 0; i < m_handlers.size(); ++i)
            {
                const string& id = m_identifiers[i];
                if(IsMatchingSymbol(nextStr, id))
                {
                    nextStr = nextStr.substr(id.size()); // filter away identifier
                    size_t nextPos;
                    long num;
                    try
                    {
                        num = stol(nextStr, &nextPos);
                    }
                    catch (std::exception) 
                    {
                        break; // invalid syntax: need number behind symbol
                    }
                    string replaceStr = (*m_handlers[i])(num);
                    replacedText += textInInterval.substr(prevPos, currentPos-prevPos) + replaceStr;
                    currentPos += id.size() + m_symbol.size() + nextPos;
                    foundId = true;
                    break;
                }
            }
            if (!foundId)
            {
                replacedText += textInInterval.substr(prevPos, (currentPos-prevPos)+m_symbol.size());
                currentPos += m_symbol.size();
            }
        }
    }
    replacedText += textInInterval.substr(prevPos);

    return replacedText;
}

}}}