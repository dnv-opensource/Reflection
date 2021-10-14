//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    TypeAspect::TypeAspect(const TypeLibraries::TypePointer& type)
        : m_type(type)
    {

    }

    TypeAspect::~TypeAspect()
    {

    }

    const Attributes::AttributeCollection& TypeAspect::GetAttributeCollection() const
    {
        if (m_type)
            return m_type->GetAttributeCollection();
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string TypeAspect::ToString() const
    {
        std::string result = "TypeAspect(";
        if (m_type)
            result += m_type->GetName();
        else
            result += "nullptr";
        result += ")";
        return result;
    }

    bool TypeAspect::operator==(const TypeAspect& other) const
    {
        return m_type == other.m_type;

    }

}}}}

