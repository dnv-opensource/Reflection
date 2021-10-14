//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionEnumFormatter.h"
#include "EnumHelper.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {

    //Actual implementation of enum formatting lies in EnumHelper.
    std::string ReflectionEnumFormatter::FormatEnum(int enumValue, const std::type_info& enumType, const Formatting::FormattingService& service) const
    {
        EnumHelper helper(enumType);
        if(!helper.GetType())
            return enumType.name() + std::string(" is not registered with the reflection type system.");
        return helper.ToString(enumValue, service);
    }

    bool ReflectionEnumFormatter::IsSupported(const std::type_info& info) const
    {
        return EnumHelper(info).GetType() != nullptr;
            
    }

}}}}

