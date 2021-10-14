#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/Config.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    //This attribute allows you to serialize a container as nested elements with attribute names.
    using Nested = std::vector<std::string>;
    class ContainerSerializationAttribute : public IAttribute
    {
    public:
        ContainerSerializationAttribute(const std::string& elementName, const std::string& attributeName)
            : m_elementName(elementName)
            , m_attributeNames(1,attributeName)
            , m_isNested(false)
        {}
        ContainerSerializationAttribute(const std::string& elementName, const std::vector<std::string>& nestedAttributeName)
        : m_elementName(elementName)
        , m_attributeNames(nestedAttributeName)
        , m_isNested(true)
        {}
        ContainerSerializationAttribute(const std::string& elementName)
            : m_elementName(elementName)
            , m_isNested(true)
        {}

        const std::string& GetElementName() const { return m_elementName; }
        const std::vector<std::string>& GetAttributeNames() const { return m_attributeNames; }
        bool IsNested() const { return m_isNested; }
    private:
        bool m_isNested;
        std::string m_elementName;
        std::vector<std::string> m_attributeNames;
    };
}}}}