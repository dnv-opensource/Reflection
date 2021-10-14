//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MockReflectionContentsService.h"
#include "Reflection/Enums/ReflectionEnumFormatter.h"
#include "Formatting/DefaultFormatterRules.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Contents { namespace Tests { 

    MockReflectionContentsService::MockReflectionContentsService(const TypeLibraries::TypeLibraryPointer& typeLibrary) 
        : m_typeLibrary(typeLibrary)
    {
        using namespace DNVS::MoFa::Formatting;
        using namespace DNVS::MoFa::Reflection::Enums;
        m_scriptFormatter.AddFormatter<IEnumFormatter>(std::make_shared<ReflectionEnumFormatter>());
        m_scriptFormatter.AddFormatter<IFormatterRules>(std::make_shared<DefaultFormatterRules>(false, IFormatterRules::Exact, true, false));

        m_displayFormatter.AddFormatter<IEnumFormatter>(std::make_shared<ReflectionEnumFormatter>());
        m_displayFormatter.AddFormatter<IFormatterRules>(std::make_shared<DefaultFormatterRules>(true, IFormatterRules::SimplifiedGui, false, true));
    }

    Objects::Object MockReflectionContentsService::Test(const std::string& text) const
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    bool MockReflectionContentsService::IsLookupExpression(const std::string& text) const
    {
        return false;
    }

    const Formatting::FormattingService& MockReflectionContentsService::GetScriptFormatter() const
    {
        return m_scriptFormatter;
    }

    const Formatting::FormattingService& MockReflectionContentsService::GetDisplayFormatter() const
    {
        return m_displayFormatter;
    }

    bool MockReflectionContentsService::TrySplitFunctionArguments(const std::string& expr, std::string& functionName, std::vector<std::string>& arguments) const
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    TypeLibraries::TypeLibraryPointer MockReflectionContentsService::GetTypeLibrary() const
    {
        return m_typeLibrary;
    }

} } } } }