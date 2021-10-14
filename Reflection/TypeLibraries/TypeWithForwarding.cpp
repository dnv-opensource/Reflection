//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeWithForwarding.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberWithArguments.h"
#include "Reflection/Classes/Class.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    TypeWithForwarding::TypeWithForwarding(TypePointer type)
        : m_type(type)
    {

    }

    std::string TypeWithForwarding::GetName() const
    {
        return m_type->GetName();
    }

    void TypeWithForwarding::SetName(const std::string& name)
    {
        m_type->SetName(name);
    }

    Members::MemberPointer TypeWithForwarding::AddMember(const std::string& name, Members::MemberPointer member)
    {
        return m_type->AddMember(name, member);
    }

    Members::MemberWithArgumentsPointer TypeWithForwarding::PrepareInvoke(const std::string& name, const std::vector<Variants::Variant>& args, Members::MemberType type /*= Members::MemberType::TypeAll*/) const
    {
        return m_type->PrepareInvoke(name, args, type);
    }

    TypeConversions::ConversionGraphPointer TypeWithForwarding::GetConversionGraph() const
    {
        return m_type->GetConversionGraph();
    }

    Members::MemberPointer TypeWithForwarding::Lookup(const std::string& name) const
    {
        return m_type->Lookup(name);
    }

    Members::MemberPointer TypeWithForwarding::Lookup(const std::string& name, const Variants::Variant& object) const
    {
        return m_type->Lookup(name, object);
    }

    std::map<std::string, Members::MemberPointer> TypeWithForwarding::GetAllMembers() const
    {
        return m_type->GetAllMembers();
    }

    void TypeWithForwarding::AddDerivedType(const std::type_info& type)
    {
        m_type->AddDerivedType(type);
    }

    void TypeWithForwarding::AddBaseType(const std::type_info& type)
    {
        m_type->AddBaseType(type);
    }

    void TypeWithForwarding::AddUniteType(const std::type_info& type)
    {
        m_type->AddUniteType(type);
    }

    std::list<std::string> TypeWithForwarding::GetUnsortedMembers() const
    {
        return m_type->GetUnsortedMembers();
    }

    const Attributes::AttributeCollection& TypeWithForwarding::GetAttributeCollection() const
    {
        return m_type->GetAttributeCollection();
    }

    Attributes::AttributeCollection& TypeWithForwarding::GetAttributeCollection()
    {
        return m_type->GetAttributeCollection();
    }

    void TypeWithForwarding::AddDocumentation(const std::string& doc)
    {
        m_type->AddDocumentation(doc);
    }

    std::set<std::type_index> TypeWithForwarding::GetBaseTypes() const
    {
        return m_type->GetBaseTypes();
    }

    std::set<std::type_index> TypeWithForwarding::GetDerivedTypes() const
    {
        return m_type->GetDerivedTypes();
    }

    std::set<std::type_index> TypeWithForwarding::GetUniteTypes() const
    {
        return m_type->GetUniteTypes();
    }

    const std::type_info& TypeWithForwarding::GetType() const
    {
        return m_type->GetType();
    }

    void DoReflect(TypeLibraryPointer typeLibrary, TypeWithForwarding**)
    {
        using namespace Classes;
        Class<TypeWithForwarding, Public<IType>, TypePointer> cls(typeLibrary, "TypeWithForwarding");
    }

}}}}

