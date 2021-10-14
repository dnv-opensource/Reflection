//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructorAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    ConstructorAspect::ConstructorAspect(const Members::MemberPointer& constructor)
        : m_constructor(constructor)
    {

    }

    ConstructorAspect::~ConstructorAspect()
    {

    }

    const Attributes::AttributeCollection& ConstructorAspect::GetAttributeCollection() const
    {
        if (m_constructor)
            return m_constructor->GetAttributeCollection();
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string ConstructorAspect::ToString() const
    {
        std::string result = "ConstructorAspect(";
        if (m_constructor)
            result += m_constructor->GetActualName();
        else
            result += "nullptr";
        result += ")";
        return result;
    }

    bool ConstructorAspect::operator==(const ConstructorAspect& other) const
    {
        return m_constructor == other.m_constructor;
    }

}}}}

