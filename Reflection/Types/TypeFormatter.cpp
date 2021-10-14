//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeFormatter.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {

    std::string TypeFormatter::FormatType(const DecoratedTypeInfo& typeInfo) const
    {
        std::string name = typeInfo.GetTypeInfo().name();
        if (name.substr(0, 6) == "class ")
            name = name.substr(6);
        else if (name.substr(0, 7) == "struct ")
            name = name.substr(7);
        else if (name.substr(0, 5) == "enum ")
            name = name.substr(5);
        return name;
    }

}}}}

