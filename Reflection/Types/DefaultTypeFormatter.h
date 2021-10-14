#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DecoratedTypeInfo.h"
#include "ITypeFormatter.h"
#include "Reflection/Config.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
#pragma warning(push)
#pragma warning(disable:4275)
    class REFLECTION_IMPORT_EXPORT DefaultTypeFormatter : public ITypeFormatter {
    public:
        DefaultTypeFormatter() {}
        virtual std::string FormatType(const DecoratedTypeInfo& typeInfo) const {
            return typeInfo.GetTypeInfo().name();
        }
    };
#pragma warning(pop)
}}}}