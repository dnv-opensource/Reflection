//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PropertyLayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    PropertyLayoutSource::PropertyLayoutSource(const Members::MemberPointer& member)
        : m_member(member)
    {

    }

    std::string PropertyLayoutSource::GetName() const
    {
        return m_member->GetName();
    }

    const Attributes::AttributeCollection& PropertyLayoutSource::GetAttributeCollection() const
    {
        return m_member->GetAttributeCollection();
    }

    Types::DecoratedTypeInfo PropertyLayoutSource::GetTypeInfo() const
    {
        if (GetSetter() && GetSetter()->GetArgumentInfo(1))
            return GetSetter()->GetArgumentInfo(1)->GetTypeInfo();
        else if (GetGetter())
            return GetGetter()->GetReturnType();
        else if(m_member)
            return m_member->GetReturnType();
        return Types::DecoratedTypeInfo();
    }

    Members::MemberPointer PropertyLayoutSource::GetGetter() const
    {
        for (Members::MemberPointer member : m_member->GetOverloads())
        {
            if (member->GetMemberType() == Members::MemberType::TypePropertyGet)
                return member;
        }
        return nullptr;
    }

    Members::MemberPointer PropertyLayoutSource::GetSetter() const
    {
        for (Members::MemberPointer member : m_member->GetOverloads())
        {
            if (member->GetMemberType() == Members::MemberType::TypePropertySet)
                return member;
        }
        return nullptr;
    }

}}}}

