#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeWithForwarding.h"
#include <set>
#include "TypeUtilities/CompareCaseLess.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {    
    /// <summary>
    /// Add support for types that support non-case sensitive lookup of members
    /// </summary>
    /// <seealso cref="TypeWithForwarding" />
    class REFLECTION_IMPORT_EXPORT TypeNotCaseSensitive : public TypeWithForwarding
    {
    public:
        TypeNotCaseSensitive(TypePointer type);
        Members::MemberPointer AddMember( const std::string& name,Members::MemberPointer member ) override;
        virtual Members::MemberWithArgumentsPointer PrepareInvoke(const std::string& name, const std::vector<Variants::Variant>& args, Members::MemberType type = Members::MemberType::TypeAll) const override;
        virtual Members::MemberPointer Lookup(const std::string& name) const override;
        virtual Members::MemberPointer Lookup(const std::string& name, const Variants::Variant& object) const override;
    private:
        const std::string& GetNameWithCorrectCase(const std::string& name) const;
        TypePointer m_actualType;
        std::set<std::string, TypeUtilities::CompareCaseLess> m_caseLessSortedNames;
    };
    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraryPointer typeLibrary, TypeNotCaseSensitive**);
}}}}