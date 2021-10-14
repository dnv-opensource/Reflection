#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FormattingService.h"
#include "IEnumFormatter.h"
#include "Config.h"
#include <type_traits>
#include <sstream>

namespace DNVS { namespace MoFa { namespace Formatting {
    //Implement ToString for double values. Will use IPrimitiveFormatter to do the formatting.
    std::string FORMATTING_IMPORT_EXPORT ToString(double value, const FormattingService& service = FormattingService());
    //Implement ToString for float values. Will use IPrimitiveFormatter to do the formatting.
    std::string FORMATTING_IMPORT_EXPORT ToString(float value, const FormattingService& service = FormattingService());
    std::string FORMATTING_IMPORT_EXPORT ToString(bool value, const FormattingService& service = FormattingService());

    //Implement ToString for integral values, int, long, bool, char etc.
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, std::string> ToString(T value, const FormattingService& service = FormattingService())
    {
        std::stringstream sstream;
        sstream << value;
        return sstream.str();
    }
    //Implement ToString for enumerations. This will always use the registered IEnumFormatter to do the actual formatting.
    //Returns error string if no IEnumFormatter is registered.
    template<typename T>
    std::enable_if_t<std::is_enum_v<T>, std::string> ToString(T value, const FormattingService& service = FormattingService())
    {
        auto enumFormatter = service.GetFormatter<IEnumFormatter>();
        if (enumFormatter)
            return enumFormatter->FormatEnum(static_cast<int>(value), typeid(T), service);
        else
            return "IEnumFormatter not registered with FormattingService";
    }
}}}