//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RecordAccessorAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    RecordAccessorAspect::RecordAccessorAspect(const Members::MemberPointer& setFunction, const Members::MemberPointer& getFunction)
        : m_setFunction(setFunction)
        , m_getFunction(getFunction)
    {

    }

    RecordAccessorAspect::~RecordAccessorAspect()
    {

    }

    const Attributes::AttributeCollection& RecordAccessorAspect::GetAttributeCollection() const
    {
        if (m_setFunction)
            return m_setFunction->GetAttributeCollection();
        else if (m_getFunction)
            return m_getFunction->GetAttributeCollection();
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string RecordAccessorAspect::ToString() const
    {
        std::string text = "RecordAccessorAspect(";
        if (m_setFunction)
            text += m_setFunction->GetName();
        else
            text += "nullptr";
        text += ", ";
        if (m_getFunction)
            text += m_getFunction->GetName();
        else
            text += "nullptr";
        text += ")";
        return text;
    }

    bool RecordAccessorAspect::operator==(const RecordAccessorAspect& other) const
    {
        return other.m_getFunction == m_getFunction && m_setFunction == other.m_setFunction;
    }

}}}}

