//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IsAliasAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    const Attributes::AttributeCollection& IsAliasAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string IsAliasAspect::ToString() const
    {
        return "IsAliasAspect()";
    }

    bool IsAliasAspect::operator==(const IsAliasAspect& other) const
    {
        return true;
    }

}}}}

