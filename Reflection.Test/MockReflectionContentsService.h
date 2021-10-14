#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Contents/IReflectionContentsService.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Contents { namespace Tests { 
    class MockReflectionContentsService : public IReflectionContentsService
    {
    public:
        MockReflectionContentsService(const TypeLibraries::TypeLibraryPointer& typeLibrary);

        Objects::Object Test(const std::string& text) const override;


        bool IsLookupExpression(const std::string& text) const override;


        const Formatting::FormattingService& GetScriptFormatter() const override;


        const Formatting::FormattingService& GetDisplayFormatter() const override;


        bool TrySplitFunctionArguments(const std::string& expr, std::string& functionName, std::vector<std::string>& arguments) const override;


        TypeLibraries::TypeLibraryPointer GetTypeLibrary() const override;

    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        Formatting::FormattingService m_scriptFormatter;
        Formatting::FormattingService m_displayFormatter;
    };
} } } } }