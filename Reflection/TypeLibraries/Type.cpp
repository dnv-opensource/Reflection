//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/Type.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberWithArguments.h"
#include "Reflection/Variants/VariantService.h"
#include "Reflection/TypeConversions/TypeConversionService.h"
#include "Reflection/Attributes/DocumentationAttribute.h"
#include "Reflection/Classes/Class.h"
#include "Formatting/FormattingService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    std::string ToString(const IType& type, const DNVS::MoFa::Formatting::FormattingService& FormattingService)
    {
        if (&type != nullptr)
            return type.GetName();
        else
            return "Type is not registered with reflection!";
    }
    Type::Type(std::shared_ptr<ITypeLibrary> typeLibrary, const std::type_info& info, const std::string& name )
        :   m_info(info)
        ,   m_name(name)
        ,   m_conversionGraph(typeLibrary->GetConversionGraph())
        ,   m_typeLibrary(typeLibrary)
    {
    }

    Members::MemberPointer Type::AddMember( const std::string& name, Members::MemberPointer member )
    {
        if (!member)
            return member;
        for(size_t i = 0; i < member->GetArity(); ++i)
        {
            if(member->GetArgumentInfo(i) && member->GetArgumentInfo(i)->HasAttributeCollection())
            {
                member->GetArgumentInfo(i)->GetAttributeCollection().AddBaseAttribute(GetAttributeCollection());
            }
        }
        Members::MemberPointer& oldMember = m_members[name];
        if (!oldMember)
        {
            oldMember = member;
            m_unsortedMembers.push_back(name);
        }
        else
        {
            Members::MemberPointer compatible = oldMember->GetCompatibleOverload(member);
            if (compatible)
                return compatible;
            oldMember = oldMember->AddOverload(member);
        }
        return member;
    }

    Members::MemberWithArgumentsPointer Type::PrepareInvoke( const std::string& name, const std::vector<Variants::Variant>& args, Members::MemberType memberType) const
    {
        Members::MemberWithArgumentsPointer memberWithArguments;
        Members::MemberPointer member = Lookup(name);
        if(!member && !args.empty()) 
            member = LookupUsingLookupCallback(name, args[0]);
        if(member) 
            memberWithArguments = member->PrepareInvoke(args, memberType);
        else 
        {
            auto typeLibrary = m_typeLibrary.lock();
            if (!typeLibrary)
                return nullptr;
            for(auto it = m_uniteTypes.begin(); it != m_uniteTypes.end(); ++it) 
            {
                TypePointer type = typeLibrary->LookupType(*it);
                if(type)
                {
                    memberWithArguments = Members::MemberWithArguments::SelectBestOverload(memberWithArguments, type->PrepareInvoke(name, args, memberType));
                }
            }
            if(memberWithArguments && memberWithArguments->IsOk()) 
                return memberWithArguments;
            for(auto it = m_baseTypes.begin(); it != m_baseTypes.end(); ++it) 
            {
                TypePointer type = typeLibrary->LookupType(*it);
                if(type) 
                {
                    memberWithArguments = Members::MemberWithArguments::SelectBestOverload(memberWithArguments, type->PrepareInvoke(name, args, memberType));
                }
            }
        }
        return memberWithArguments;
    }

    std::string Type::GetName() const
    {
        return m_name;
    }

    TypeConversions::ConversionGraphPointer Type::GetConversionGraph() const
    {
        return m_conversionGraph;
    }

    Members::MemberPointer Type::Lookup( const std::string& name ) const
    {
        auto it = m_members.find(name);
        if(it == m_members.end()) 
        {
            return Members::MemberPointer();
        }
        return it->second;
    }

    Members::MemberPointer Type::Lookup(const std::string& name, const Variants::Variant& object) const
    {
        Members::MemberPointer member = Lookup(name);
        if (!member)
            member = LookupUsingLookupCallback(name, object);
        if (!member)
        {
            auto typeLibrary = m_typeLibrary.lock();
            if (!typeLibrary)
                return nullptr;
            for (auto it = m_uniteTypes.begin(); it != m_uniteTypes.end(); ++it)
            {
                TypePointer type = typeLibrary->LookupType(*it);
                if (type)
                {
                    member = type->Lookup(name, object);
                    if (member)
                        return member;
                }
            }
            for (auto it = m_baseTypes.begin(); it != m_baseTypes.end(); ++it)
            {
                TypePointer type = typeLibrary->LookupType(*it);
                if (type)
                {
                    member = type->Lookup(name, object);
                    if (member)
                        return member;
                }
            }
        }
        return member;
    }

    std::map<std::string, Members::MemberPointer> Type::GetAllMembers() const
    {
        return m_members;
    }

    std::list<std::string> Type::GetUnsortedMembers() const
    {
        return m_unsortedMembers;
    }

    void Type::SetName(const std::string& name)
    {
        m_name = name;
    }

    void Type::AddDerivedType( const std::type_info& type )
    {
        if (type == m_info)
            return;
        m_derivedTypes.insert(type);
    }

    void Type::AddBaseType( const std::type_info& type )
    {
        if (type == m_info)
            return;
        m_baseTypes.insert(type);
    }
    void Type::AddUniteType(const std::type_info& type)
    {
        if (type == m_info)
            return;
        if(m_uniteTypes.insert(type).second)
        {
            auto typeLibrary = m_typeLibrary.lock();
            if (typeLibrary)
                typeLibrary->CreateType(type, "")->AddUniteType(m_info);
        }
    }

    const Attributes::AttributeCollection& Type::GetAttributeCollection() const
    {
        return m_attributeCollection;
    }

    Attributes::AttributeCollection& Type::GetAttributeCollection()
    {
        return m_attributeCollection;
    }

    void Type::AddDocumentation(const std::string& doc)
    {
        GetAttributeCollection().AddAttribute(Attributes::DocumentationAttribute(doc));
    }

    std::set<std::type_index> Type::GetBaseTypes() const
    {
        return m_baseTypes;
    }

    Members::MemberPointer Type::LookupUsingLookupCallback(const std::string& name, const Variants::Variant& arg) const
    {
        auto it = m_members.find("__lookup__");
        if(it == m_members.end()) 
            return Members::MemberPointer();
        Members::MemberPointer member = it->second;
        std::vector<Variants::Variant> lookupArgs(3);
        lookupArgs[0] = arg;
        lookupArgs[1] = Variants::VariantService::ReflectType<std::string>(name);
        lookupArgs[2] = Variants::VariantService::ReflectType<std::shared_ptr<TypeLibraries::ITypeLibrary>>(m_typeLibrary.lock());
        Members::MemberWithArgumentsPointer result = member->PrepareInvoke(lookupArgs, Members::MemberType::TypeFunction);
        if(result && result->IsOk()) 
        {
            Variants::Variant outcome = result->Invoke();
            if(TypeConversions::TypeConversionService(m_conversionGraph).IsConvertibleTo<Members::MemberPointer>(outcome)) 
            {
                return TypeConversions::TypeConversionService(m_conversionGraph).As<Members::MemberPointer>(outcome);
            }
        }
        return Members::MemberPointer();
    }

    std::set<std::type_index> Type::GetDerivedTypes() const
    {
        return m_derivedTypes;
    }

    std::set<std::type_index> Type::GetUniteTypes() const
    {
        return m_uniteTypes;
    }

    const std::type_info& Type::GetType() const
    {
        return m_info;
    }

    void DoReflect(TypeLibraryPointer typeLibrary, IType**)
    {
        using namespace Classes;
        Class<IType, TypePointer> cls(typeLibrary, "IType");
        RegisterToStringFunction(cls);
    }

    void DoReflect(TypeLibraryPointer typeLibrary, Type**)
    {
        using namespace Classes;
        Class<Type, Public<IType>, TypePointer> cls(typeLibrary, "Type");
    }
}}}}