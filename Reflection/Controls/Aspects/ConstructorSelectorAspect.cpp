//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructorSelectorAspect.h"
#include "Reflection/Types/DecoratedTypeFormatter.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    ConstructorSelectorAspect::ConstructorSelectorAspect(const Types::DecoratedTypeInfo& decoratedTypeInfo, const std::set<Members::MemberPointer>& constructors)
        : m_decoratedTypeInfo(decoratedTypeInfo)
        , m_constructors(constructors)
    {

    }

    ConstructorSelectorAspect::~ConstructorSelectorAspect()
    {

    }

    const Attributes::AttributeCollection& ConstructorSelectorAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string ConstructorSelectorAspect::ToString() const
    {
        std::string result = "ConstructorSelectorAspect(";
        result += Types::DecoratedTypeFormatter(std::make_shared<Types::DefaultTypeFormatter>()).FormatType(m_decoratedTypeInfo);
        result += ", {";
        bool isFirst = true;
        for (const auto& member : m_constructors)
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

    bool ConstructorSelectorAspect::operator==(const ConstructorSelectorAspect& other) const
    {
        return m_decoratedTypeInfo == other.m_decoratedTypeInfo && m_constructors == other.m_constructors;
    }

}}}}

