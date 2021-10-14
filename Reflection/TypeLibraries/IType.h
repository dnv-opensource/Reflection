#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/Members/MemberWithArgumentsFwd.h"
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/Members/MemberType.h"

#include "Reflection/Variants/Variant.h"
#include "Reflection/TypeConversions/IConversionGraph.h"
#include "TypeLibraryPointer.h"

#include <string>
#include <memory>
#include <map>
#include <list>
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    class AttributeCollection;
}}}}

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {
    
    /// <summary>
    /// This class represents a type in the reflection library. You can query it about the inheritance graph,
    /// lookup members, invoke members and lookup general attributes of the type.
    /// It is the backbone of the reflection library.
    /// </summary>
    class REFLECTION_IMPORT_EXPORT IType
    {
    public:
        virtual ~IType() {}
        virtual std::string GetName() const = 0;
        virtual void SetName(const std::string& name) = 0;
        //The returned member is guaranteed to have the exact same type data as the input member.
        virtual Members::MemberPointer AddMember(const std::string& name, Members::MemberPointer member) = 0;
        virtual Members::MemberWithArgumentsPointer PrepareInvoke(const std::string& name, const std::vector<Variants::Variant>& args, Members::MemberType type = Members::MemberType::TypeAll) const = 0;
        virtual Members::MemberPointer Lookup(const std::string& name) const = 0;
        virtual Members::MemberPointer Lookup(const std::string& name, const Variants::Variant& object) const = 0;
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const = 0;
        virtual void AddDerivedType(const std::type_info& type) = 0;
        virtual void AddBaseType(const std::type_info& type) = 0;
        virtual void AddUniteType(const std::type_info& type) = 0;
        virtual std::map<std::string, Members::MemberPointer> GetAllMembers() const = 0;
        virtual std::list<std::string> GetUnsortedMembers() const = 0;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const = 0;
        virtual Attributes::AttributeCollection& GetAttributeCollection() = 0;
        virtual void AddDocumentation(const std::string& doc) = 0;
        virtual std::set<std::type_index> GetBaseTypes() const = 0;
        virtual std::set<std::type_index> GetDerivedTypes() const = 0;
        virtual std::set<std::type_index> GetUniteTypes() const = 0;
        virtual const std::type_info& GetType() const = 0;
    };
    typedef std::shared_ptr<IType> TypePointer;
    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraryPointer typeLibrary, IType**);

}}}}
