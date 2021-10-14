//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "GroupAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    GroupAspect::GroupAspect(const std::string& groupName)
        : m_groupName(groupName)
    {
    }

    const Attributes::AttributeCollection& GroupAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string GroupAspect::ToString() const
    {
        return "GroupAspect(\"" + m_groupName + "\")";
    }

    const std::string& GroupAspect::GetGroupName() const
    {
        return m_groupName;
    }

    bool GroupAspect::operator==(const GroupAspect& other) const
    {
        return m_groupName == other.m_groupName;
    }

}}}}

