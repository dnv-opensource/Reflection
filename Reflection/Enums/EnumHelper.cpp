//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "EnumHelper.h"
#include "Reflection/Variants/InternalVariantService.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/Method.h"
#include "Formatting/IdentityFormatter.h"
#include "Reflection/Objects/Object.h"
#include "Services/ServiceProvider.h"
#include "Formatting/DefaultFormatterRules.h"
#include "Reflection/Attributes/SerializationAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/AliasAttribute.h"
#include "Reflection/Attributes/VisibleAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {

    EnumHelper::EnumHelper(TypeLibraries::TypeLibraryPointer typeLibrary, const std::type_index& enumType) 
    {
        if (typeLibrary)
            m_type = typeLibrary->LookupType(enumType);
    }

    EnumHelper::EnumHelper(const TypeLibraries::TypePointer& type)
        : m_type(type)
    {

    }

    EnumHelper::EnumHelper(const std::type_index& enumType)
    {
        auto typeLibrary = Services::ServiceProvider::Instance().TryGetService<TypeLibraries::ITypeLibrary>();
        if (!typeLibrary)
            return;
        m_type = typeLibrary->LookupType(enumType);
    }

    TypeLibraries::TypePointer EnumHelper::GetType() const
    {
        return m_type;
    }

    std::string EnumHelper::GetName() const
    {
        return m_type->GetName();
    }

    std::string EnumHelper::GetEnumValueName(int enumValue)
    {
        if (!m_type)
            return "";
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            auto result = GetEnumValue(imember->second);
            if(result.second && result.first == enumValue)
                return imember->second->GetName();
        }
        return "";
    }

    std::string EnumHelper::GetFormattedEnumValueName(const std::string enumName)
    {
        if (!m_type)
            return "";
        auto member = m_type->Lookup(enumName);
        if (member)
            return member->GetFormatter().Format(enumName);
        else
            return enumName;
    }

    std::string EnumHelper::GetFormattedEnumValueName(int enumValue)
    {
        return GetFormatter(enumValue).Format(GetEnumValueName(enumValue));
    }

    const Formatting::IStringFormatter& EnumHelper::GetFormatter(int enumValue)
    {
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            Members::MemberPointer member = imember->second;
            auto result = GetEnumValue(member);
            if (result.second && enumValue == result.first)
                return member->GetFormatter();
        }
        return Formatting::IdentityFormatter::GetIdentityFormatter();
    }

    void EnumHelper::SetFormatter(const Formatting::IStringFormatter& formatter)
    {
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            Members::MemberPointer member = imember->second;
            
            if (imember->second->GetMemberType() == Members::MemberType::TypePropertyGet && &member->GetFormatter() == &Formatting::IdentityFormatter::GetIdentityFormatter())
            {
                boost::intrusive_ptr<Members::Method> method = boost::dynamic_pointer_cast<Members::Method>(member);
                if (method)
                    method->SetFormatter(formatter);
            }
        }
    }

    Members::MemberPointer EnumHelper::GetMember(const Objects::Object& o) const
    {
        if (!m_type)
            return nullptr;
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            Members::MemberPointer member = imember->second;
            try {
                if (member->GetMemberType() == Members::MemberType::TypePropertyGet && member->GetArgumentInfo(0)->GetTypeInfo() == Types::TypeId<Variants::Variant>())
                {
                    if (Objects::Object(o.GetTypeLibrary(), member->FastInvoke(std::vector<Variants::Variant>(1, Variants::Variant()))) == o)
                        return member;
                }
            }
            catch(std::exception&)
            { }
        }
        return nullptr;
    }

    Members::MemberPointer EnumHelper::GetMember(int enumValue) const
    {
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            Members::MemberPointer member = imember->second;
            try {
                std::pair<int,bool> result = GetEnumValue(member);
                if (result.second && result.first == enumValue)
                    return member;
            }
            catch (std::exception&)
            {
            }
        }
        return nullptr;
    }

    std::list<Members::MemberPointer> EnumHelper::GetAllEnumMembers() const
    {
        std::list<Members::MemberPointer> enumMembers;
        auto members = m_type->GetAllMembers();
        for (auto member : members)
        {
            if (member.second->GetMemberType() == Members::MemberType::TypePropertyGet)
                enumMembers.push_back(member.second);
        }
        return enumMembers;
    }

    std::set<long> EnumHelper::GetAllEnumValues() const
    {
        std::set<long> values;
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            auto result = GetEnumValue(imember->second);
            //Don't add items with alias.
            if(result.second && (imember->second->GetName().empty() || imember->first == imember->second->GetName()))
                values.insert(result.first);
        }
        return values;
    }

    std::pair<int, bool> EnumHelper::GetEnumValue(const std::string& name) const
    {
        auto member = m_type->Lookup(name);
        if (!member)
            return std::make_pair(-1, false);
        return GetEnumValue(member);
    }

    std::pair<int, bool> EnumHelper::GetEnumValue(const Members::MemberPointer& member) const
    {
        if (member->GetMemberType() == Members::MemberType::TypePropertyGet)
        {
            int value = Variants::InternalVariantService::UnreflectUnchecked<int>(member->FastInvoke(std::vector<Variants::Variant>(1, Variants::Variant())));
            return std::make_pair(value, true);
        }
        else
            return std::make_pair(-1, false);
    }

    std::list<Variants::Variant> EnumHelper::GetAllReflectedEnumValues() const
    {
        std::list<Variants::Variant> values;
        auto members = m_type->GetAllMembers();
        for (const std::string& name : m_type->GetUnsortedMembers())
        {
            auto imember = members.find(name);
            if (imember != members.end())
            {
                if (imember->first == imember->second->GetName() && imember->second->GetMemberType() == Members::MemberType::TypePropertyGet)
                {
                    auto member = imember->second;
                    if (member->GetAttributeCollection().HasAttribute<Attributes::VisibleAttribute>())
                    {
                        const auto& visibleAttribute = member->GetAttributeCollection().GetAttribute<Attributes::VisibleAttribute>();
                        if (visibleAttribute.GetStringArguments().empty() && !visibleAttribute.IsVisible({}))
                            continue;
                    }
                    if (!member->GetAttributeCollection().HasAttribute<Attributes::AliasAttribute>())
                        values.push_back(member->FastInvoke(std::vector<Variants::Variant>(1)));
                }
            }
        }
        return values;
    }

    std::string EnumHelper::ToString(int enumValue, const Formatting::FormattingService& service) const
    {
        if (!m_type)
            return "";
        auto nameAndMember = GetNameAndMember(enumValue);
        return ToString(nameAndMember.second, service);
    }

    std::string EnumHelper::ToString(const Members::MemberPointer& member, const Formatting::FormattingService& service) const
    {
        if (!member)
            return "";
        using namespace Formatting;
        std::shared_ptr<IFormatterRules> formatterRules = service.GetFormatterOrDefault<IFormatterRules, DefaultFormatterRules>();
        if (formatterRules->GetExpressionRepresentation() == IFormatterRules::Serialization && member->GetAttributeCollection().HasAttribute<Reflection::Attributes::SerializationAttribute>())
        {
            return member->GetAttributeCollection().GetAttribute<Reflection::Attributes::SerializationAttribute>().GetName();
        }
        std::string name = member->GetName();
        //Allow short hand notation, "ssFromStructure" becomes "From Structure"
        if (!formatterRules->RequireValidScript())
        {
            if (member->GetAttributeCollection().HasAttribute<Attributes::AliasAttribute>())
            {
                auto alias = member->GetAttributeCollection().GetAttribute<Attributes::AliasAttribute>().GetAlias();
                name = ToString(Variants::InternalVariantService::UnreflectUnchecked<int>(alias), service);
            }
            else
                name = member->GetFormatter().Format(name);
        }
            


        //If this is an internal enumeration, we may want to enclose the expression with parenthesis.
        if (formatterRules->ShouldEncloseExpression() && member->GetName() != GetNameOfMember(member) && !member->GetName().empty())
            name = "(" + name + ")";

        return name;
    }

    std::pair<std::string, Members::MemberPointer> EnumHelper::GetNameAndMember(int enumValue) const
    {
        auto members = m_type->GetAllMembers();
        for (auto imember = members.begin(); imember != members.end(); ++imember)
        {
            Members::MemberPointer member = imember->second;
            auto result = GetEnumValue(member);
            if (result.second && enumValue == result.first)
                return *imember;
        }
        return std::make_pair("", nullptr);
    }

    int EnumHelper::TryGetAlias(Members::MemberPointer member, int value) const
    {
        if (member->GetAttributeCollection().HasAttribute<Attributes::AliasAttribute>())
            return Variants::InternalVariantService::UnreflectUnchecked<int>(member->GetAttributeCollection().GetAttribute<Attributes::AliasAttribute>().GetAlias());
        else
            return value;
    }

    std::string EnumHelper::GetNameOfMember(const Members::MemberPointer& member) const
    {
        if (!m_type)
            return "";
        for (auto existingMember : m_type->GetAllMembers())
        {
            if(existingMember.second == member)
                return existingMember.first;
        }
        return "";
    }

}}}}


