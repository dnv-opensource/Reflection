#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "AttributeCollection.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

    inline void AddAttributes(AttributeCollection& coll){}

    template<typename Attribute, typename... Attributes>
    void AddAttributes(AttributeCollection& coll, Attribute&& attribute, Attributes&&... attributes)
    {
        coll.AddAttribute(std::forward<Attribute>(attribute));
        AddAttributes(coll, std::forward<Attributes>(attributes)...);
    }
    template<typename T, typename... Attributes>
    AttributeCollection OverrideAttributeCollection(TypeLibraries::TypeLibraryPointer typeLibrary, Attributes&&... attributes)
    {
        AttributeCollection coll;
        auto type = typeLibrary->LookupType(typeid(T));
        if (type)
            coll.AddBaseAttribute(type->GetAttributeCollection());
        AddAttributes(coll, std::forward<Attributes>(attributes)...);
        return coll;
    }
    template<typename T, typename... Attributes>
    AttributeCollection OverrideAttributeCollection(Attributes&&... attributes)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        return OverrideAttributeCollection<T>(typeLibrary, std::forward<Attributes>(attributes)...);
    }

    template<typename T>
    const T* GetPointerToAttributeOrNull()
    {
        return nullptr;
    }
    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const AttributeCollection& collection, const Args&... args)
    {
        if (collection.HasAttribute<T>())
            return &collection.GetAttribute<T>();
        return GetPointerToAttributeOrNull<T>(args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const Members::MemberPointer& member, const Args&... args)
    {
        if (member)
            return GetPointerToAttributeOrNull<T>(member->GetAttributeCollection(), args...);
        return GetPointerToAttributeOrNull<T>(args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const TypeLibraries::TypePointer& type, const Args&... args)
    {
        if (type)
            return GetPointerToAttributeOrNull<T>(type->GetAttributeCollection(), args...);
        else
            return GetPointerToAttributeOrNull<T>(args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const std::type_index& typeInfo, const Args&... args)
    {
        return GetPointerToAttributeOrNull<T>(Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>(), typeInfo, args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const type_info& typeInfo, const Args&... args)
    {
        return GetPointerToAttributeOrNull<T>(std::type_index(typeInfo), args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const Types::DecoratedTypeInfo& typeInfo, const Args&... args)
    {
        return GetPointerToAttributeOrNull<T>(typeInfo.GetTypeInfo(), args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::type_index& typeInfo, const Args&... args)
    {
        if (!typeLibrary)
            return nullptr;
        return GetPointerToAttributeOrNull<T>(typeLibrary->LookupType(typeInfo), args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const TypeLibraries::TypeLibraryPointer& typeLibrary, const type_info& typeInfo, const Args&... args)
    {
        return GetPointerToAttributeOrNull<T>(typeLibrary, std::type_index(typeInfo), args...);
    }

    template<typename T, typename... Args>
    const T* GetPointerToAttributeOrNull(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& typeInfo, const Args&... args)
    {
        return GetPointerToAttributeOrNull<T>(typeLibrary, typeInfo.GetTypeInfo(), args...);
    }
}}}}
