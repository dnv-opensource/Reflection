//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ToString.h"
#include "DefaultPrimitiveFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    std::string ToString(double value, const FormattingService& service)
    {
        return service.GetFormatterOrDefault<IPrimitiveFormatter, DefaultPrimitiveFormatter>()->FormatDouble(value, service);
    }
    std::string ToString(float value, const FormattingService& service)
    {
        return service.GetFormatterOrDefault<IPrimitiveFormatter, DefaultPrimitiveFormatter>()->FormatFloat(value, service);
    }

    std::string ToString(bool value, const FormattingService& service /*= FormattingService()*/)
    {
        switch (value)
        {
        case true:
            return "true";
        default:
            return "false";
        }
    }

}}}