#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DecoratedTypeInfo.h"
#include <string>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
    class ITypeFormatter {
    public:
        virtual ~ITypeFormatter() {}
        virtual std::string FormatType(const DecoratedTypeInfo& typeInfo) const = 0;
    };
}}}}