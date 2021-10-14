#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttributeWrapper.h"
#include "IAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    template<typename AttributeT>
    class AttributeWrapper : public IAttributeWrapper
    {
    public:
        AttributeWrapper(const AttributeT& attribute) : m_attribute(attribute) {}
        AttributeWrapper(AttributeT&& attribute) : m_attribute(std::move(attribute)) {}
        const AttributeT& Get() const { return m_attribute; }
    private:
        AttributeT m_attribute;
    };

    class DynamicAttributeWrapper : public IAttributeWrapper
    {
    public:
        DynamicAttributeWrapper(const std::shared_ptr<IAttribute>& attribute) : m_attribute(attribute) {}
        const std::shared_ptr<IAttribute>& Get() const { return m_attribute; }
    private:
        std::shared_ptr<IAttribute> m_attribute;
    };
}}}}
