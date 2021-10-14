#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFormatter.h"
#include <string>

namespace DNVS { namespace MoFa { namespace Formatting {
    //Interface to do formatting of enums.
    class FORMATTING_IMPORT_EXPORT IEnumFormatter : public IFormatter
    {
    public:
        virtual std::string FormatEnum(int enumValue, const std::type_info& enumType, const FormattingService& service) const = 0;
        virtual bool IsSupported(const std::type_info& info) const = 0;
    };
}}}
