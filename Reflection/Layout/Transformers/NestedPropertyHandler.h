#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Layout/LayoutContext.h"
#include "Reflection/Config.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "TypeUtilities/CompareCaseLess.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class REFLECTION_IMPORT_EXPORT NestedProperty {
    public:
        NestedProperty(const TypeLibraries::TypeLibraryPointer& typeLibrary, const TypeLibraries::TypePointer& type, const std::string& name);
        NestedProperty* Add(const std::string& name);
        const NestedProperty* Lookup(const std::string& name) const;
        const LayoutContext& GetContext() const;
        using NestedProperties = std::map<std::string, NestedProperty, TypeUtilities::CompareCaseLess>;
        const NestedProperties& GetPropertyMap() const { return m_nestedProperties; }
        void Finalize();
    private:
        bool IsValidConstructor(const Members::MemberPointer& member) const;
        LayoutContext m_context;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        TypeLibraries::TypePointer m_parentType;
        std::map<std::string, NestedProperty, TypeUtilities::CompareCaseLess> m_nestedProperties;
    };
    class REFLECTION_IMPORT_EXPORT NestedPropertyHandler {
    public:
        NestedPropertyHandler(const TypeLibraries::TypeLibraryPointer& typeLibrary, const TypeLibraries::TypePointer& type);
        NestedProperty* CreateProperty(const std::string& name);
        const NestedProperty* LookupProperty(const std::string& name) const;
        using NestedProperties = std::map<std::string, NestedProperty, TypeUtilities::CompareCaseLess>;
        const NestedProperties& GetPropertyMap() const { return m_nestedProperties; }
        void Finalize();
    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        TypeLibraries::TypePointer m_type;
        NestedProperties m_nestedProperties;
    };
}}}}