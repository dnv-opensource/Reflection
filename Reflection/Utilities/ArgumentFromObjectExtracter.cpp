//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ArgumentFromObjectExtracter.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Attributes/GenerateConstructorArgumentsAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/CaptionAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {

    ArgumentFromObjectExtracter::ArgumentFromObjectExtracter(TypeLibraries::TypeLibraryPointer typeLibrary, Members::MemberPointer constructor, size_t argumentIndex)
        : m_typeLibrary(typeLibrary)
        , m_argumentIndex(argumentIndex)
        , m_generateConstructorArgumentsAttribute(nullptr)
        , m_constructor(constructor)
    {
        if (!m_typeLibrary)
            return;
        if (!constructor)
            return;
        if (m_argumentIndex >= constructor->GetArity())
            return;
        std::string name = constructor->GetArgumentInfo(m_argumentIndex)->GetName();
        if (TryPopulatePropertyGetSet(constructor->GetReturnType().GetTypeInfo(), name))
            return;
        TryPopulateFromGenerateConstructorArgumentsAttribute(constructor);
    }

    ArgumentFromObjectExtracter::ArgumentFromObjectExtracter(TypeLibraries::TypeLibraryPointer typeLibrary, Members::MemberPointer constructor, Members::MemberPointer getter, Members::MemberPointer setter)
        : m_typeLibrary(typeLibrary)
        , m_getter(getter)
        , m_setter(setter)
        , m_constructor(constructor)
        , m_generateConstructorArgumentsAttribute(nullptr)
        , m_argumentIndex(0)
    {
        if (!m_typeLibrary)
            return;        
        if (!m_getter && m_setter)
            DetectConstructorArgumentIndexFromArgumentName(m_setter->GetName());
        if(!m_getter && !m_setter)
            TryPopulateFromGenerateConstructorArgumentsAttribute(constructor);
    }

    ArgumentFromObjectExtracter::ArgumentFromObjectExtracter(TypeLibraries::TypeLibraryPointer typeLibrary, const std::type_index& typeIndex, const std::string& argumentName, Members::MemberPointer constructor)
        : m_typeLibrary(typeLibrary)
        , m_argumentIndex(0)
        , m_generateConstructorArgumentsAttribute(nullptr)
        , m_constructor(constructor)
    {
        if (!m_typeLibrary)
            return;
        if (TryPopulatePropertyGetSet(typeIndex, argumentName))
            return;
        DetectConstructorArgumentIndexFromArgumentName(argumentName);
    }

    void ArgumentFromObjectExtracter::DetectConstructorArgumentIndexFromArgumentName(const std::string& argumentName)
    {
        if (!m_constructor)
            return;
        for (size_t i = 0; i < m_constructor->GetArity(); ++i)
        {
            //Do a caseless string compare between argumentName and the name of constructor argument i.
            if (_strcmpi(m_constructor->GetArgumentInfo(i)->GetName().c_str(), argumentName.c_str()) == 0)
            {
                m_argumentIndex = i;
                TryPopulateFromGenerateConstructorArgumentsAttribute(m_constructor);
                return;
            }
        }
    }

    ArgumentFromObjectExtracter::~ArgumentFromObjectExtracter()
    {

    }

    Variants::Variant ArgumentFromObjectExtracter::operator()(const Variants::Variant& variant) const
    {
        Objects::Object object(m_typeLibrary, variant);
        if (m_getter && object.IsConvertibleTo(m_getter->GetArgumentInfo(0)->GetTypeInfo()))
            return m_getter->Invoke({ variant });
        else if (m_generateConstructorArgumentsAttribute)
        {
            std::vector<Variants::Variant> args;
            if (m_generateConstructorArgumentsAttribute->GenerateConstructorArguments(object, args))
            {
                if (m_argumentIndex < args.size())
                    return args[m_argumentIndex];
            }
        }
        if (m_constructor && m_constructor->GetArgumentInfo(m_argumentIndex) && m_constructor->GetArgumentInfo(m_argumentIndex)->HasDefault())
            return m_constructor->GetArgumentInfo(m_argumentIndex)->GetDefault();
        return Variants::Variant();
    }

    Members::MemberPointer ArgumentFromObjectExtracter::GetGetter() const
    {
        return m_getter;
    }

    Members::MemberPointer ArgumentFromObjectExtracter::GetSetter() const
    {
        return m_setter;
    }   

    std::string ArgumentFromObjectExtracter::GetName() const
    {
        if (m_getter)
            return m_getter->GetName();
        if (m_setter)
            return m_setter->GetName();
        if (!m_constructor || m_argumentIndex >= m_constructor->GetArity())
            return "";
        return m_constructor->GetArgumentInfo(m_argumentIndex)->GetName();
    }

    std::string ArgumentFromObjectExtracter::GetCaption() const
    {
        if (m_getter && m_getter->GetAttributeCollection().HasAttribute<Attributes::CaptionAttribute>())
            return m_getter->GetAttributeCollection().GetAttribute<Attributes::CaptionAttribute>().GetCaption();
        if (m_setter && m_setter->GetAttributeCollection().HasAttribute<Attributes::CaptionAttribute>())
            return m_setter->GetAttributeCollection().GetAttribute<Attributes::CaptionAttribute>().GetCaption();
        auto info = m_constructor->GetArgumentInfo(m_argumentIndex);
        if (info && info->GetAttributeCollection().HasAttribute<Attributes::CaptionAttribute>())
            return info->GetAttributeCollection().GetAttribute<Attributes::CaptionAttribute>().GetCaption();
        return "";
    }

    bool ArgumentFromObjectExtracter::HasSetter() const
    {
        return m_setter != nullptr;
    }


    Types::DecoratedTypeInfo ArgumentFromObjectExtracter::GetType() const
    {
        if (m_getter)
            return m_getter->GetReturnType();
        if (m_setter)
            return m_setter->GetArgumentInfo(1)->GetTypeInfo();
        if (!m_constructor || m_argumentIndex >= m_constructor->GetArity())
            return Types::DecoratedTypeInfo();
        return m_constructor->GetArgumentInfo(m_argumentIndex)->GetTypeInfo();
    }

    bool ArgumentFromObjectExtracter::TryPopulatePropertyGetSet(const std::type_index& typeIndex, const std::string& name)
    {
        auto type = m_typeLibrary->LookupType(typeIndex);
        if (!type)
            return false;
        auto property = type->Lookup(name, Variants::Variant());
        if (!property)
            return false;
        for (auto propertyOverload : property->GetOverloads())
        {
            if (propertyOverload->GetMemberType() == Members::MemberType::TypePropertyGet)
            {
                m_getter = propertyOverload;
            }
            if (propertyOverload->GetMemberType() == Members::MemberType::TypePropertySet)
            {
                m_setter = propertyOverload;
            }
        }
        return m_getter != nullptr;
    }

    bool ArgumentFromObjectExtracter::TryPopulateFromGenerateConstructorArgumentsAttribute(Members::MemberPointer constructor)
    {
        if (!constructor)
            return false;
        if (!constructor->GetAttributeCollection().HasAttribute<Attributes::GenerateConstructorArgumentsAttribute>())
            return false;
        else
        {
            m_generateConstructorArgumentsAttribute = &constructor->GetAttributeCollection().GetAttribute<Attributes::GenerateConstructorArgumentsAttribute>();
            return true;
        }
    }

}}}}

