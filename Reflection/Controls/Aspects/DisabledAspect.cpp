//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DisabledAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    
    const Attributes::AttributeCollection& DisabledAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string DisabledAspect::ToString() const
    {
        return "DisableAspect()";
    }

    bool DisabledAspect::operator==(const DisabledAspect& other) const
    {
        return true;
    }

}}}}

