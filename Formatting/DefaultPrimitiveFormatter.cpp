//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultPrimitiveFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    DefaultPrimitiveFormatter::DefaultPrimitiveFormatter(int doublePrecision /*= 10*/, std::ios_base::fmtflags flags /*= 0*/)
        : m_doublePrecision(doublePrecision)
    {
        m_format[0] = '%';
        m_format[1] = '.';
        m_format[2] = '*';
        switch (flags)
        {
        case std::ios_base::fixed:
            m_format[3] = 'f';
            break;
        case std::ios_base::scientific:
            m_format[3] = 'e'; 
            break;
        case std::ios_base::floatfield:
        default:
            m_format[3] = 'g';
            break;
        }
        m_format[4] = 0;
    }

    std::string DefaultPrimitiveFormatter::FormatDouble(double number, const FormattingService& formattingService) const
    {
        char numberAsString[200];
        sprintf_s(numberAsString, 200, m_format, m_doublePrecision, number);
        return numberAsString;
    }

    std::string DefaultPrimitiveFormatter::FormatFloat(float number, const FormattingService& formattingService) const
    {
        char numberAsString[200];
        sprintf_s(numberAsString, 200, m_format, m_doublePrecision, number);
        return numberAsString;
    }

}}}

