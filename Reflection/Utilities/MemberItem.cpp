//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberItem.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/CaptionAttribute.h"
#include "sstream"
#include "Reflection/Attributes/ActionAttribute.h"
#include "Reflection/Attributes/ListAsConstructorAttribute.h"
#include "Reflection/Attributes/UtilityClassAttribute.h"
#include "Reflection/Attributes/GenerateConstructorArgumentsAttribute.h"
#include "Formatting/DefaultFormatterRules.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    

    MemberItem::MemberItem(Members::MemberPointer member, TypeLibraries::TypeLibraryPointer typeLibrary)
        : m_member(member)
        , m_typeLibrary(typeLibrary)
    {
    }

    bool MemberItem::operator==(const MemberItem& other) const
    {
        return m_member == other.m_member;
    }

    Members::MemberPointer MemberItem::GetMember() const
    {
        return m_member;
    }

    std::string MemberItem::GetOrGenerateName(size_t index) const
    {
        auto argument = m_member->GetArgumentInfo(index);
        std::string name = argument->GetName();
        if (!name.empty())
            return name;
        if (m_member->GetMemberType() == Members::MemberType::TypePropertyGet)
            return m_member->GetName();

        TypeLibraries::TypePointer type;
        if (m_typeLibrary)
            type = m_typeLibrary->LookupType(argument->GetTypeInfo().GetTypeInfo());
        std::stringstream stream;
        if (!type)
            stream << "Unnamed argument " << index;
        else
        {
            stream << "Unnamed " << type->GetName() << index;
        }
        return stream.str();
    }

    bool MemberItem::AreConstructorArgumentBoundToObjectProperties() const
    {
        if (!m_typeLibrary)
            return false;
        auto type = m_typeLibrary->LookupType(m_member->GetReturnType().GetTypeInfo());
        if (!type)
            return false;
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            std::string name = m_member->GetArgumentInfo(i)->GetName();
            auto member = type->Lookup(name, Variants::Variant());
            if (member && (member->GetMemberType() & Members::MemberType::TypePropertyGet) == Members::MemberType::TypePropertyGet)
                continue;
            if(m_member->GetArgumentInfo(i)->HasDefault())
                continue;
            if(m_member->GetAttributeCollection().HasAttribute<Attributes::GenerateConstructorArgumentsAttribute>())
                continue;
            return false;
        }
        return true;
    }

    bool MemberItem::AreConstructorArgumentsNamed() const
    {
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            if (m_member->GetArgumentInfo(i)->GetName().empty())
                return false;
        }
        return true;
    }

    bool MemberItem::IsConstructor() const
    {
        if (!m_member)
            return false;
        if ((m_member->GetMemberType() & Members::MemberType::TypeConstructor) == Members::MemberType::TypeConstructor)
            return true;
        else if (m_member->GetAttributeCollection().HasAttribute<Attributes::ListAsConstructorAttribute>())
        {
            switch (m_member->GetAttributeCollection().GetAttribute<Attributes::ListAsConstructorAttribute>().GetOption())
            {
                case Attributes::ListAsConstructorAttribute::ListNever:
                    return false;
                default:
                    return true;
            }
        }
        return false;
    }

    bool MemberItem::IsUtilityClassFunction() const
    {
        if (!m_member)
            return false;
        auto type = m_typeLibrary->LookupType(GetTypeInfo().GetTypeInfo());
        if (!type)
            return false;
        return type->GetAttributeCollection().HasAttribute<Attributes::UtilityClassAttribute>();
    }

    Types::DecoratedTypeInfo MemberItem::GetTypeInfo() const
    {
        if (!m_member)
            return Types::DecoratedTypeInfo();
        switch (m_member->GetMemberType())
        {
            case Members::MemberType::TypeConstructor:
            case Members::MemberType::TypeFunction:
            case Members::MemberType::TypeOperator:
                return m_member->GetReturnType();
            case Members::MemberType::TypeMemberFunction:
            case Members::MemberType::TypePropertyGet:
            case Members::MemberType::TypePropertySet:
            case Members::MemberType::TypePropertyGetSet:
                return m_member->GetArgumentInfo(0)->GetTypeInfo();
            default:
                return Types::DecoratedTypeInfo();
        }
    }

    TypeLibraries::TypeLibraryPointer MemberItem::GetTypeLibrary() const
    {
        return m_typeLibrary;
    }



    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MemberItem**)
    {
        using namespace Classes;
        Class<MemberItem> cls(typeLibrary, "MemberItem");
        RegisterToStringFunction(cls);
        cls.Operator(This.Const == This.Const);
    }

    std::string ToString(const MemberItem& item, const Formatting::FormattingService& formattingService)
    {
        auto rules = formattingService.GetFormatterOrDefault<Formatting::IFormatterRules, Formatting::DefaultFormatterRules>();
        if (!item.GetMember())
            return "null";
        if (!rules->RequireValidScript())
        {
            if (item.GetMember()->GetAttributeCollection().HasAttribute<Attributes::ActionAttribute>())
            {
                return item.GetMember()->GetAttributeCollection().GetAttribute<Attributes::ActionAttribute>().GetName();
            }
            if (item.GetMember()->GetAttributeCollection().HasAttribute<Attributes::CaptionAttribute>())
            {
                return item.GetMember()->GetAttributeCollection().GetAttribute<Attributes::CaptionAttribute>().GetCaption();
            }
            else if (item.GetMember()->GetMemberType() & Members::MemberType::TypePropertyGetSet)
            {
                return item.GetMember()->GetName();
            }
        }
        std::vector<std::string> arguments;
        for (size_t i = 0; i < item.GetMember()->GetArity(); ++i)
        {
            arguments.push_back(item.GetOrGenerateName(i));
        }
        return item.GetMember()->ToString(arguments);
    }

    LexicalMemberItemSorter::LexicalMemberItemSorter(const Formatting::FormattingService& service /*= Formatting::FormattingService()*/)
        : m_formattingService(service)
    {

    }

    bool LexicalMemberItemSorter::operator()(const MemberItem& lhs, const MemberItem& rhs) const
    {
        return ToString(lhs, m_formattingService) < ToString(rhs, m_formattingService);
    }

}}}}


