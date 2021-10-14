//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionArgumentAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    FunctionArgumentAspect::FunctionArgumentAspect(const Members::MemberPointer& member, size_t index)
        : m_member(member)
        , m_index(index)
    {

    }

    FunctionArgumentAspect::~FunctionArgumentAspect()
    {

    }

    const Attributes::AttributeCollection& FunctionArgumentAspect::GetAttributeCollection() const
    {
        if (m_member && m_member->GetArgumentInfo(m_index))
            return m_member->GetArgumentInfo(m_index)->GetAttributeCollection();
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string FunctionArgumentAspect::ToString() const
    {
        std::string result = "FunctionArgumentAspect(";
        if (m_member)
            result += m_member->GetActualName();
        else
            result += "nullptr";
        result += ", " + std::to_string(m_index) + ")";
        return result;
    }

    Contents::Content FunctionArgumentAspect::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify)
    {
        if (trySimplify)
            return parent.TryGetArgument(m_member, m_index);
        else
            return Contents::Content();
    }

    bool FunctionArgumentAspect::operator==(const FunctionArgumentAspect& other) const
    {
        return m_member == other.m_member && m_index == other.m_index;
    }

}}}}