#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "TypeLibraryPointer.h"
#include <typeindex>
namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {
    class REFLECTION_IMPORT_EXPORT ExtendedTypeTraits {
    public:
        ExtendedTypeTraits(const TypeLibraryPointer& typeLibrary);
        bool IsBaseOf(const std::type_index& base, const std::type_index& derived) const;
        bool HasConstructors(const std::type_index& typeIndex) const;
        bool HasDerivedTypes(const std::type_index& typeIndex) const;
    private:
        TypeLibraryPointer m_typeLibrary;
    };
}}}}
