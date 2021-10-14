#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IType.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {    
    /// <summary>
    /// This is a base class that forwards all methods to another type.
    /// We use if for <cref = "TypeNotCaseSensitive"/> and <cref = "TypeWithGlobalMemberRegistration"/>
    /// </summary>
    /// <seealso cref="IType" />
    class REFLECTION_IMPORT_EXPORT TypeWithForwarding : public IType
    {
    public:
        TypeWithForwarding(TypePointer type);
        virtual std::string GetName() const override;
        virtual void SetName(const std::string& name) override;
        virtual Members::MemberPointer AddMember( const std::string& name, Members::MemberPointer member ) override;
        virtual Members::MemberWithArgumentsPointer PrepareInvoke( const std::string& name, const std::vector<Variants::Variant>& args, Members::MemberType type = Members::MemberType::TypeAll) const override;
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const override;
        virtual Members::MemberPointer Lookup(const std::string& name) const override;
        virtual Members::MemberPointer Lookup(const std::string& name, const Variants::Variant& object) const override;
        virtual std::map<std::string, Members::MemberPointer> GetAllMembers() const override;
        virtual void AddDerivedType(const std::type_info& type) override;
        virtual void AddBaseType(const std::type_info& type) override;

        virtual void AddUniteType( const std::type_info& type ) override;
        virtual std::list<std::string> GetUnsortedMembers() const override;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const override;
        virtual Attributes::AttributeCollection& GetAttributeCollection() override;
        virtual void AddDocumentation(const std::string& doc) override;
        virtual std::set<std::type_index> GetBaseTypes() const override;
        virtual std::set<std::type_index> GetDerivedTypes() const override;
        virtual std::set<std::type_index> GetUniteTypes() const override;
        virtual const std::type_info& GetType() const override;
    private:
        TypePointer m_type;
    };

    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraryPointer typeLibrary, TypeWithForwarding**);
}}}}