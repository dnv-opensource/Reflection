//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultArgumentExtracter.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/DefaultAttribute.h"
#include "Reflection/Attributes/DefaultTypeAttribute.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "boost\algorithm\string.hpp"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {

    DefaultArgumentExtracter::DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member)
        : DefaultArgumentExtracter(typeLibrary, member, nullptr, 0)
    {
    }

    DefaultArgumentExtracter::DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& constructor, const std::string& argumentName)
        : DefaultArgumentExtracter(typeLibrary, nullptr, constructor, GetIndexOfNamedConstructorArgument(constructor, argumentName))
    {

    }

    DefaultArgumentExtracter::DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& constructor, size_t argumentIndex)
        : DefaultArgumentExtracter(typeLibrary, nullptr, constructor, argumentIndex)
    {

    }

    DefaultArgumentExtracter::DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, const Members::MemberPointer& constructor, size_t argumentIndex)
        : m_typeLibrary(typeLibrary)
        , m_member(member)
        , m_constructor(constructor)
        , m_argumentIndex(argumentIndex)
    {
    }

    DefaultArgumentExtracter::DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, const Members::MemberPointer& constructor, const std::string& argumentName)
        : DefaultArgumentExtracter(typeLibrary, member, constructor, GetIndexOfNamedConstructorArgument(constructor, argumentName))
    {

    }

    Variants::Variant DefaultArgumentExtracter::TryGetDefaultValue(const std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess>& additionalArguments) const
    {
        Variants::Variant defaultValue = TryGetDefaultValueFromMember(additionalArguments);
        if (!defaultValue.IsValid())
            defaultValue = TryGetDefaultValueFromConstructorArgument();

        return defaultValue;
    }

    Variants::Variant DefaultArgumentExtracter::TryGetDefaultValue(const Attributes::DefaultAttribute& defaultAttribute, const std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess>& additionalArguments) const
    {
        if (!defaultAttribute.HasFunction())
            return Variants::Variant();
        std::vector<Objects::Object> args;
        for (const std::string& arg : defaultAttribute.GetStringArguments())
        {
            auto it = additionalArguments.find(arg);
            if (it != additionalArguments.end())
                args.push_back(Objects::Object(m_typeLibrary, it->second));
            else
                return Variants::Variant();
        }
        try {
            return defaultAttribute.InvokeFunction(args);
        }
        catch(...)
        {
            return Variants::Variant();
        }
    }

    Variants::Variant DefaultArgumentExtracter::TryGetDefaultValueFromMember(const std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess>& additionalArguments) const
    {
        if (!m_member)
            return Variants::Variant();
        using namespace Attributes;
        if (m_member->GetAttributeCollection().HasAttribute<DefaultAttribute>())
        {
            Variants::Variant defaultValue = TryGetDefaultValue(m_member->GetAttributeCollection().GetAttribute<DefaultAttribute>(), additionalArguments);
            if (defaultValue.IsValid())
                return defaultValue;
        }
        if (m_typeLibrary && m_member->GetAttributeCollection().HasAttribute<DefaultTypeAttribute>())
        {
            const DefaultTypeAttribute& defaultTypeAttribute = m_member->GetAttributeCollection().GetAttribute<DefaultTypeAttribute>();
            if(defaultTypeAttribute.HasDefaultType())
            {
                const std::type_info& type = defaultTypeAttribute.GetDefaultType();
                return Variants::VariantService::Reflect(m_typeLibrary->LookupType(type));
            }
        }
        return Variants::Variant();
    }

    size_t DefaultArgumentExtracter::GetIndexOfNamedConstructorArgument(const Members::MemberPointer& constructor, const std::string& argumentName)
    {
        if (!constructor)
            return std::numeric_limits<size_t>::max();
        for (size_t i = 0; i < constructor->GetArity(); ++i)
        {
            if(boost::iequals(argumentName, constructor->GetArgumentInfo(i)->GetName()))
                return i;
        }
        return std::numeric_limits<size_t>::max();
    }

    Variants::Variant DefaultArgumentExtracter::TryGetDefaultValueFromConstructorArgument() const
    {
        if (!m_constructor || m_argumentIndex >= m_constructor->GetArity() || !m_constructor->GetArgumentInfo(m_argumentIndex)->HasDefault())
            return Variants::Variant();
        else
            return m_constructor->GetArgumentInfo(m_argumentIndex)->GetDefault();
    }

}}}}

