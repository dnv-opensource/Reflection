#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>
#include "IType.h"
#include "IMember.h"
#include "..\jsScript_config.h"
#include "Reflection\TypeLibraries\IType.h"
#include "Reflection\Members\IMemberFwd.h"
class jsValue;
class jsTypeInfo;

namespace DNVS { namespace MoFa { namespace Doc {
    class JSSCRIPT_IMPORT_EXPORT TypeMap 
    {
    public:
        TypeMap(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary);
        TypePointer GetOrCreateFromPrototype(jsValue* value);
        TypePointer GetOrCreateFromReflectionType(const Reflection::TypeLibraries::TypePointer& type);
        TypePointer GetOrCreateFromTypeIndex(const std::type_index& type);
        MemberPointer CreateFromFunctionTypeInfo(jsTypeInfo* typeInfo);
        MemberPointer CreateFromReflectionMember(const Reflection::Members::MemberPointer& member);
        typedef std::map<std::string, TypePointer> Types;
        typedef Types::const_iterator const_iterator;
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator find(const std::string& name) const;
        typedef std::map<jsValue*, TypePointer> PrototypeToTypeMap;
        typedef std::map<Reflection::TypeLibraries::TypePointer, TypePointer> ReflectionTypeTpTypeMap;
        const PrototypeToTypeMap& GetPrototypeToTypeMap() const;
        const Types& GetSortedTypes() const;
        TypePointer GetOrCreateGlobalReflectionType();
        void CreateReflectionTypes();
    private:
        DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
        PrototypeToTypeMap m_prototypeToType;
        ReflectionTypeTpTypeMap m_reflectionTypeToType;
        Types m_types;
    };
}}}