//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeMap.h"
#include "ScriptType.h"
#include "ScriptMember.h"
#include "..\jsValue.h"
#include "ReflectionType.h"
#include "Reflection\TypeLibraries\ITypeLibrary.h"
#include "ReflectionMember.h"
#include "Reflection\Members\GlobalType.h"

namespace DNVS { namespace MoFa { namespace Doc {

    TypeMap::TypeMap(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary)
        : m_typeLibrary(typeLibrary)
    {

    }

    TypePointer TypeMap::GetOrCreateFromPrototype(jsValue* value)
    {
        if (!value)
            return nullptr;
        auto it = m_prototypeToType.find(value);
        if (it != m_prototypeToType.end())
            return it->second;
        TypePointer type = std::make_shared<ScriptType>(value, *this);
        m_prototypeToType[value] = type;
        m_types[type->GetName()] = type;
        return type;
    }

    TypePointer TypeMap::GetOrCreateFromReflectionType(const Reflection::TypeLibraries::TypePointer& reflectionType)
    {
        if (!reflectionType)
            return nullptr;
        auto it = m_reflectionTypeToType.find(reflectionType);
        if (it != m_reflectionTypeToType.end())
            return it->second;
        TypePointer type = std::make_shared<ReflectionType>(reflectionType, *this);
        m_reflectionTypeToType[reflectionType] = type;
        //If this type already exists, don't overwrite it.
        m_types.insert(std::make_pair(type->GetName(), type));
        return type;
    }

    TypePointer TypeMap::GetOrCreateFromTypeIndex(const std::type_index& type)
    {
        auto reflectionType = m_typeLibrary->LookupType(type);
        if (!reflectionType)
            return nullptr;
        else
            return GetOrCreateFromReflectionType(reflectionType);
    }

    MemberPointer TypeMap::CreateFromFunctionTypeInfo(jsTypeInfo* typeInfo)
    {
        return std::make_shared<ScriptMember>(typeInfo);
    }

    MemberPointer TypeMap::CreateFromReflectionMember(const Reflection::Members::MemberPointer& member)
    {
        return std::make_shared<ReflectionMember>(member, *this);
    }

    const TypeMap::PrototypeToTypeMap& TypeMap::GetPrototypeToTypeMap() const
    {
        return m_prototypeToType;
    }

    const TypeMap::Types& TypeMap::GetSortedTypes() const
    {
        return m_types;
    }

    TypePointer TypeMap::GetOrCreateGlobalReflectionType()
    {
        return GetOrCreateFromTypeIndex(typeid(DNVS::MoFa::Reflection::Members::GlobalType));
    }

    void TypeMap::CreateReflectionTypes()
    {
        for (auto typeInMap : m_typeLibrary->GetAllTypes())
        {
            if (typeInMap.first != typeid(DNVS::MoFa::Reflection::Members::GlobalType))
                GetOrCreateFromReflectionType(typeInMap.second);
        }
    }

}}}

