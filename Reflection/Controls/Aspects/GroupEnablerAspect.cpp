//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "GroupEnablerAspect.h"
#include "Reflection/Attributes/GroupAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    GroupEnablerAspect::GroupEnablerAspect(const Attributes::GroupEnablerAttribute& attribute, const std::string& groupName)
    {
        m_localCollection.AddAttribute(Attributes::GroupEnablerAttribute(attribute));
        m_localCollection.AddAttribute(Attributes::GroupAttribute(groupName));
    }

    const Attributes::AttributeCollection& GroupEnablerAspect::GetAttributeCollection() const
    {
        return m_localCollection;
    }

    std::string GroupEnablerAspect::ToString() const
    {
        return "GroupEnablerAspect(...)";
    }

    bool GroupEnablerAspect::operator==(const GroupEnablerAspect& other) const
    {
        return true;
    }

}}}}

