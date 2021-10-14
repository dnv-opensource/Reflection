//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ExpandActionAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    ExpandActionAspect::ExpandActionAspect(bool defaultExpanded)
        : m_defaultExpanded(defaultExpanded)
    {
    }

    const Attributes::AttributeCollection& ExpandActionAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string ExpandActionAspect::ToString() const
    {
        return "ExpandActionAspect(" + std::to_string(m_defaultExpanded) + ")";
    }

    bool ExpandActionAspect::IsDefaultExpanded() const
    {
        return m_defaultExpanded;
    }

    bool ExpandActionAspect::operator==(const ExpandActionAspect& other) const
    {
        return m_defaultExpanded == other.m_defaultExpanded;
    }

}}}}

