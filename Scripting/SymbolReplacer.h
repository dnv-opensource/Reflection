#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <functional>
#include <vector>
#include <list>
#include <memory>
#include "Config.h"

namespace DNVS { namespace MoFa { namespace Scripting {

    class SCRIPTING_IMPORT_EXPORT SymbolReplacer
    {
    public:
        SymbolReplacer(std::string symbol);
        virtual ~SymbolReplacer() {}

        void AddHandler(const std::string& postfix, const std::function<std::string(long)>& handler);
        virtual std::string& Replace(const std::string& text);
        virtual std::string& Replace(const char* text, size_t length);
        std::string GetSymbol() const;
    private:
        std::list<std::pair<size_t,size_t>> FindSearchIntervals(const std::string& text);
        std::string ReplaceTextInInterval(const std::string& text, std::pair<size_t, size_t> interval);
        std::pair<size_t, size_t> GetQuotationPos(const std::string& text, size_t currentPos);
        size_t findEndQuotation(const std::string& text, size_t start, bool checkSingleQuotationMark);

        enum QuotationMemory { empty, singleQuote, doubleQuote };

        std::string m_symbol;
        std::vector<std::shared_ptr<std::function<std::string(long)>>> m_handlers;
        std::vector<std::string> m_identifiers;
        QuotationMemory m_previousGotStuckInText;
        std::string m_replacement;
    };

    typedef std::shared_ptr<SymbolReplacer> SymbolReplacerPointer;

}}}