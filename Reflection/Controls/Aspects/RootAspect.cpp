//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RootAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    const Attributes::AttributeCollection& RootAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string RootAspect::ToString() const
    {
        return "RootAspect()";
    }

    Contents::Content RootAspect::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify /*= true*/)
    {
        return parent;
    }

    Contents::Content RootAspect::TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value)
    {
        return parent.Assign(value);
    }

    bool RootAspect::SupportAssign() const
    {
        return true;
    }

    bool RootAspect::operator==(const RootAspect& other) const
    {
        return true;
    }

}}}}

