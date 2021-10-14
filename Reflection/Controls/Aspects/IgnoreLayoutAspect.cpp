//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IgnoreLayoutAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    IgnoreLayoutAspect::IgnoreLayoutAspect()
    {

    }

    const Attributes::AttributeCollection& IgnoreLayoutAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string IgnoreLayoutAspect::ToString() const
    {
        return "IgnoreLayoutAspect()";
    }

    bool IgnoreLayoutAspect::operator==(const IgnoreLayoutAspect& other) const
    {
        return true;
    }

}}}}

