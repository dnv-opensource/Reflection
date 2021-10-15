#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAutoCompletionContext.h"
#include <memory>
#include "jsAutoCompletion.h"

class JSSCRIPT_IMPORT_EXPORT jsAutoCompletionService
{
public:
    jsAutoCompletionService(std::shared_ptr<DNVS::MoFa::Scripting::IAutoCompletionContext> context);
    std::shared_ptr<jsAutoCompletion> TryAutocomplete(const std::string& str, bool functionCompletion);
private:
    bool isIdentifierStart(char c);
    bool isIdentifierPart(char c);
    int findIdentifierStart(const std::string& str, int index);
    int parseMember(const std::string& str, int index);
    std::shared_ptr<DNVS::MoFa::Scripting::IAutoCompletionContext> m_context;
};