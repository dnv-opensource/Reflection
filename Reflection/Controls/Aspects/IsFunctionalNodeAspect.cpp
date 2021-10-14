//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IsFunctionalNodeAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    const Attributes::AttributeCollection& IsFunctionalNodeAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string IsFunctionalNodeAspect::ToString() const
    {
        return "IsFunctionalNodeAspect()";
    }

    bool IsFunctionalNodeAspect::operator==(const IsFunctionalNodeAspect& other) const
    {
        return true;
    }
}}}}

