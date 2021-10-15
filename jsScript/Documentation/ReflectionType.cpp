//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionType.h"
#include "TypeMap.h"
#include "Reflection/Attributes/DocumentationAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Members/IMember.h"
#include "ReflectionMember.h"
#include "../jsTypeInfo.h"

namespace DNVS { namespace MoFa { namespace Doc {
    using namespace Reflection::Attributes;

    ReflectionType::ReflectionType(const Reflection::TypeLibraries::TypePointer& type, TypeMap& typeMap)
        : m_type(type)
        , m_typeMap(typeMap)
    {

    }

    std::string ReflectionType::GetName() const
    {
        return m_type->GetName();
    }

    std::set<TypePointer> ReflectionType::GetBaseClasses() const
    {
        std::set<TypePointer> bases;
        for (const auto& typeIndex : m_type->GetBaseTypes())
        {
            bases.insert(m_typeMap.GetOrCreateFromTypeIndex(typeIndex));
        }
        return bases;
    }

    std::set<TypePointer> ReflectionType::GetDerivedClasses() const
    {
        std::set<TypePointer> bases;
        for (const auto& typeIndex : m_type->GetDerivedTypes())
        {
            bases.insert(m_typeMap.GetOrCreateFromTypeIndex(typeIndex));
        }
        return bases;
    }

    bool ReflectionType::HasDocumentation() const
    {
        return m_type->GetAttributeCollection().HasAttribute<DocumentationAttribute>();
    }

    std::string ReflectionType::GetDocumentation() const
    {
        if (!HasDocumentation())
            return "";
        else
            return m_type->GetAttributeCollection().GetAttribute<DocumentationAttribute>().GetDocumentation();
    }

    std::string ReflectionType::GetExample() const
    {
        return "";
    }

    std::vector<jsExampleInfo> ReflectionType::GetExamples() const
    {
        return {};
    }

    std::vector<MemberPointer> ReflectionType::GetMembers(IMember::MemberType memberType) const
    {        
        std::vector<MemberPointer> wrappedMembers;
        auto members = m_type->GetAllMembers();
        for (auto member : members)
        {
            if ((ReflectionMember::ConvertMemberType(member.second->GetMemberType())&memberType) == 0)
                continue;
            if ((member.second->GetMemberType()&Reflection::Members::MemberType::TypePropertyGetSet))
            {
                wrappedMembers.push_back(m_typeMap.CreateFromReflectionMember(member.second));
            }
            else
                for (auto overload : member.second->GetOverloads())
                {
                    if ((ReflectionMember::ConvertMemberType(overload->GetMemberType())&memberType) == 0)
                        continue;
                    wrappedMembers.push_back(m_typeMap.CreateFromReflectionMember(overload));
                }
        }
        return wrappedMembers;
    }

    std::string ReflectionType::ToString(bool sortable) const
    {
        if (sortable)
            return "z::" + GetName();
        else
            return "";
    }

    std::string ReflectionType::GetNamespace() const
    {
        return "";
    }

}}}

