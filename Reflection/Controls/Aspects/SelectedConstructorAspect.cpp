//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SelectedConstructorAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    SelectedConstructorAspect::SelectedConstructorAspect(const Members::MemberPointer& constructor)
        : m_constructor(constructor)
    {

    }

    SelectedConstructorAspect::~SelectedConstructorAspect()
    {

    }

    const Attributes::AttributeCollection& SelectedConstructorAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string SelectedConstructorAspect::ToString() const
    {
        std::string result = "SelectedConstructorAspect(";
        if (m_constructor)
            result += m_constructor->GetActualName();
        else
            result += "nullptr";
        result += ")";
        return result;
    }

    Contents::Content SelectedConstructorAspect::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify /*= true*/)
    {
        std::vector<Contents::Content> result;
        for (const Contents::Content& content : parent.Split())
        {
            if (content.TryGetBestMember() == m_constructor)
                result.push_back(content);
        }
        return Contents::Content(result);
    }

    bool SelectedConstructorAspect::SupportAssign() const
    {
        return true;
    }

    Contents::Content SelectedConstructorAspect::TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value)
    {
        return parent.Assign(value);
    }

    bool SelectedConstructorAspect::operator==(const SelectedConstructorAspect& other) const
    {
        return m_constructor == other.m_constructor;
    }

}}}}

