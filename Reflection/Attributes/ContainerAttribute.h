#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class ContainerAttribute : public IAttribute {
    public:
        ContainerAttribute(const Types::DecoratedTypeInfo& valueType, bool hasSorting) : m_valueType(valueType) , m_hasSorting(hasSorting) {}
        const Types::DecoratedTypeInfo& GetValueType() const { return m_valueType; }
        bool HasSorting() const { return m_hasSorting; }
    private:
        Types::DecoratedTypeInfo m_valueType;
        bool m_hasSorting;
    };
}}}}
