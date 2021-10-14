//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionSelectorAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    FunctionSelectorAspect::FunctionSelectorAspect(const std::list<Members::MemberPointer>& functions)
        : m_functions(functions)
    {

    }

    FunctionSelectorAspect::~FunctionSelectorAspect()
    {

    }

    const Attributes::AttributeCollection& FunctionSelectorAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string FunctionSelectorAspect::ToString() const
    {
        std::string result = "FunctionSelectorAspect(";
        result += "{";
        bool isFirst = true;
        for (const auto& member : m_functions)
        {
            if (!isFirst)
                result += ", ";
            else
                isFirst = false;
            result += member->GetActualName();
        }
        result + "})";
        return result;
    }

    bool FunctionSelectorAspect::operator==(const FunctionSelectorAspect& other) const
    {
        return m_functions == other.m_functions;
    }

}}}}

