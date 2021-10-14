//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PropertyAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    PropertyAspect::PropertyAspect(const Members::MemberPointer& property) : m_property(property)
    {

    }

    PropertyAspect::~PropertyAspect()
    {

    }

    const Attributes::AttributeCollection& PropertyAspect::GetAttributeCollection() const
    {
        if (m_property)
            return m_property->GetAttributeCollection();
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    bool PropertyAspect::operator==(const PropertyAspect& other) const
    {
        return m_property == other.m_property;
    }

    std::string PropertyAspect::ToString() const
    {
        std::string result = "PropertyAspect(";
        if (m_property)
            result += m_property->GetName();
        else
            result += "nullptr";
        result += ")";
        return result;
    }

    Members::MemberPointer PropertyAspect::GetGetter() const
    {
        for (const auto& overload : m_property->GetOverloads())
            if (overload->GetMemberType() == Members::MemberType::TypePropertyGet)
                return overload;
        return nullptr;
    }

    Members::MemberPointer PropertyAspect::GetSetter() const
    {
        for (const auto& overload : m_property->GetOverloads())
            if (overload->GetMemberType() == Members::MemberType::TypePropertySet)
                return overload;
        return nullptr;
    }

    Contents::Content PropertyAspect::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify)
    {
        if (Members::MemberPointer getter = GetGetter())
        {
            Contents::Content result = parent.TryInvoke(getter, {}, trySimplify);
            if (result.IsValid())
                return result;
            else
            {
                if (trySimplify)
                    result = parent.TryInvoke(getter, {}, false);
                return Contents::Content(result.GetScriptText(), getter->GetReturnType());
            }
        }
        return Contents::Content();
    }

    Contents::Content PropertyAspect::TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value)
    {
        if (Members::MemberPointer setter = GetSetter())
        {
            return parent.TryInvoke(setter, { value });
        }
        return Contents::Content();
    }

    bool PropertyAspect::SupportAssign() const
    {
        return GetSetter() != nullptr;
    }

}}}}

