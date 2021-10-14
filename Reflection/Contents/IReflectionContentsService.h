#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Formatting/FormattingService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Contents {
    class IReflectionContentsService {
    public:
        virtual ~IReflectionContentsService() {}
        virtual Objects::Object Test(const std::string& text) const = 0;
        virtual bool IsLookupExpression(const std::string& text) const = 0;
        virtual const DNVS::MoFa::Formatting::FormattingService& GetScriptFormatter() const = 0;
        virtual const DNVS::MoFa::Formatting::FormattingService& GetDisplayFormatter() const = 0;
        virtual bool TrySplitFunctionArguments(const std::string& expr, std::string& functionName, std::vector<std::string>& arguments) const = 0;
        virtual TypeLibraries::TypeLibraryPointer GetTypeLibrary() const = 0;
    };
} } } }