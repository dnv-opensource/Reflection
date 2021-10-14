#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFormatter.h"
#include <string>

namespace DNVS { namespace MoFa { namespace Formatting {
    //Interface to do formatting of double and float numbers.
    class FORMATTING_IMPORT_EXPORT IPrimitiveFormatter : public IFormatter {
    public:
        virtual std::string FormatDouble(double number, const FormattingService& formattingService) const = 0;
        virtual std::string FormatFloat(float number, const FormattingService& formattingService) const = 0;
    };
}}}
