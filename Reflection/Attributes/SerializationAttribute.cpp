//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SerializationAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    const SerializationOptions SerializationOptions::None = 0;
    const SerializationOptions SerializationOptions::Postpone = 1;
    const SerializationOptions SerializationOptions::Optional = 2;
    const SerializationOptions SerializationOptions::Attribute = 4;

    const SerializationOptions::SerializationIgnore SerializationOptions::Ignore;

    SerializationOptions::SerializationIgnore::operator SerializationOptions() const
    {
        return SerializationOptions(8);
    }

}}}}

