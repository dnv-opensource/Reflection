#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "IType.h"
#include "Reflection/Members/IMember.h"
#include <map>
#include "ITypeLibrary.h"
#include <set>
#include <list>
#include "Reflection/Attributes/AttributeCollection.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {
    class REFLECTION_IMPORT_EXPORT Type : public IType {
    public:
        Type(std::shared_ptr<ITypeLibrary> typeLibrary,const std::type_info& info,const std::string& name);
        std::string GetName() const override;
        void SetName(const std::string& name) override;
        Members::MemberPointer AddMember( const std::string& name,Members::MemberPointer member ) override;
        Members::MemberWithArgumentsPointer PrepareInvoke( const std::string& name,const std::vector<Variants::Variant>& args,Members::MemberType type=Members::MemberType::TypeAll) const override;
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const override;
        virtual Members::MemberPointer Lookup(const std::string& name) const override;
        virtual Members::MemberPointer Lookup(const std::string& name, const Variants::Variant& object) const override;
        virtual std::map<std::string, Members::MemberPointer> GetAllMembers() const override;
        virtual std::list<std::string> GetUnsortedMembers() const override;
        ///Looks up name using a custom __lookup__ funtion. This requires the user to register a function with the name "__lookup__".
        Members::MemberPointer LookupUsingLookupCallback( const std::string& name,const Variants::Variant& arg) const;
        void AddDerivedType(const std::type_info& type) override;
        void AddBaseType(const std::type_info& type) override;
        void AddUniteType(const std::type_info& type) override;
        typedef std::set<std::type_index> TypeInfoSet;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const override;
        virtual Attributes::AttributeCollection& GetAttributeCollection() override;
        virtual void AddDocumentation(const std::string& doc) override;
        virtual std::set<std::type_index> GetBaseTypes() const override;
        virtual std::set<std::type_index> GetDerivedTypes() const override;
        virtual std::set<std::type_index> GetUniteTypes() const override;
        virtual const std::type_info& GetType() const override;
    private:
        std::map<std::string,Members::MemberPointer> m_members;
        std::list<std::string> m_unsortedMembers;
        const std::type_info& m_info;
        std::string m_name;
        TypeInfoSet m_baseTypes;
        TypeInfoSet m_uniteTypes;
        TypeInfoSet m_derivedTypes;
        std::weak_ptr<ITypeLibrary> m_typeLibrary;
        TypeConversions::ConversionGraphPointer m_conversionGraph;
        Attributes::AttributeCollection m_attributeCollection;
    };

    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraryPointer typeLibrary, Type**);
}}}}