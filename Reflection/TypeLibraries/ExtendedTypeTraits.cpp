//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ExtendedTypeTraits.h"
#include "ITypeLibrary.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    ExtendedTypeTraits::ExtendedTypeTraits(const TypeLibraryPointer& typeLibrary) : m_typeLibrary(typeLibrary)
    {

    }

    bool ExtendedTypeTraits::IsBaseOf(const std::type_index& base, const std::type_index& derived) const
    {
        if (derived == base)
            return true;
        auto type = m_typeLibrary->LookupType(derived);
        if (!type)
            return false;
        for (const auto& derivedBase : type->GetBaseTypes())
        {
            if (IsBaseOf(base, derivedBase))
                return true;
        }
        return false;
    }

    bool ExtendedTypeTraits::HasConstructors(const std::type_index& typeIndex) const
    {
        auto type = m_typeLibrary->LookupType(typeIndex);
        if (!type)
            return false;
        return type->Lookup(Members::ConstructorString) != nullptr;
    }

    bool ExtendedTypeTraits::HasDerivedTypes(const std::type_index& typeIndex) const
    {
        auto type = m_typeLibrary->LookupType(typeIndex);
        if (!type)
            return false;
        return !type->GetDerivedTypes().empty();
    }

}}}}


