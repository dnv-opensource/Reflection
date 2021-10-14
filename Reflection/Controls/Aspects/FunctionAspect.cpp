//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    FunctionAspect::FunctionAspect(const Members::MemberPointer& function)
        : m_function(function)
    {

    }

    FunctionAspect::~FunctionAspect()
    {

    }

    const Attributes::AttributeCollection& FunctionAspect::GetAttributeCollection() const
    {
        if (m_function)
            return m_function->GetAttributeCollection();
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string FunctionAspect::ToString() const
    {
        std::string result = "FunctionAspect(";
        if (m_function)
            result += m_function->GetActualName();
        else
            result += "nullptr";
        result += ")";
        return result;
    }

    bool FunctionAspect::operator==(const FunctionAspect& other) const
    {
        return m_function == other.m_function;
    }
}}}}

