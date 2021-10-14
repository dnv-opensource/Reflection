#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IPrimitiveFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT DefaultPrimitiveFormatter : public IPrimitiveFormatter {
    public:
        DefaultPrimitiveFormatter(int doublePrecision = 10, std::ios_base::fmtflags flags = 0);
        virtual std::string FormatDouble(double number, const FormattingService& formattingService) const;
        virtual std::string FormatFloat(float number, const FormattingService& formattingService) const;
    private:
        int m_doublePrecision;
        char m_format[5];
    };
}}}