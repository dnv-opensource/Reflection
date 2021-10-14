//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ArgumentInfo.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/DefaultAttribute.h"
#include "Reflection/Attributes/DocumentationAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    ArgumentInfo::ArgumentInfo(size_t index, const Types::DecoratedTypeInfo& type) 
        : m_name("arg" + boost::lexical_cast<std::string>(index))
        , m_type(type)
    {

    }

    ArgumentInfo::ArgumentInfo(const std::string& name, const Types::DecoratedTypeInfo& type) 
        : m_name(name)
        , m_type(type)
    {

    }

    ArgumentInfo::ArgumentInfo(const std::string& name, const std::string& documentation) 
        : m_name(name)
    {
        if(!documentation.empty())
            GetAttributeCollection().AddAttribute<Attributes::DocumentationAttribute>(documentation);
    }

    ArgumentInfo::ArgumentInfo(const std::string& typeAlias, const std::string& name, const std::string& documentation) 
        : m_typeAlias(typeAlias)
        , m_name(name)
    {
        if (!documentation.empty())
            GetAttributeCollection().AddAttribute<Attributes::DocumentationAttribute>(documentation);
    }

    ArgumentInfo::~ArgumentInfo()
    {

    }

    void ArgumentInfo::SetName(const std::string& name)
    {
        m_name = name;
    }

    std::string ArgumentInfo::GetName() const
    {
        return m_name;
    }

    Types::DecoratedTypeInfo ArgumentInfo::GetTypeInfo() const
    {
        return m_type;
    }

    std::string ArgumentInfo::TypeAlias() const
    {
        return m_typeAlias;
    }

    void ArgumentInfo::SetTypeInfo(const Types::DecoratedTypeInfo& info)
    {
        m_type = info;
    }

    std::string ArgumentInfo::GetDocumentation() const
    {
        if (GetAttributeCollection().HasAttribute<Attributes::DocumentationAttribute>())
            return GetAttributeCollection().GetAttribute<Attributes::DocumentationAttribute>().GetDocumentation();
        else
            return "";
    }

    bool ArgumentInfo::HasDefault() const
    {
        return GetAttributeCollection().HasAttribute<Attributes::DefaultAttribute>();
    }

    Variants::Variant ArgumentInfo::GetDefault() const
    {
        if (HasDefault())
            return GetAttributeCollection().GetAttribute<Attributes::DefaultAttribute>().InvokeFunction({});
        else
            return Variants::Variant();
    }

    void ArgumentInfo::SetDefault(const Variants::Variant& defaultValue)
    {
        GetAttributeCollection().AddAttribute<Attributes::DefaultAttribute>(defaultValue);
    }

    const Attributes::AttributeCollection& ArgumentInfo::GetAttributeCollection() const
    {
        if (m_attributeCollection)
            return *m_attributeCollection;
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    Attributes::AttributeCollection& ArgumentInfo::GetAttributeCollection()
    {
        if (!m_attributeCollection)
            m_attributeCollection.reset(new Attributes::AttributeCollection);
        return *m_attributeCollection;
    }

    bool ArgumentInfo::HasAttributeCollection() const
    {
        return m_attributeCollection != nullptr;
    }

}}}}


