#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeWithForwarding.h"
#include "TypeLibraryPointer.h"
#include "IType.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {    
    /// <summary>
    /// This is a class that provide base classes for a type.
    /// We use if for <cref = "TypeNotCaseSensitive"/> and <cref = "TypeWithGlobalMemberRegistration"/>
    /// </summary>
    /// <seealso cref="TypeWithForwarding" />

    class REFLECTION_IMPORT_EXPORT TypeWithBases : public TypeWithForwarding
    {
    public:
        TypeWithBases(const TypePointer& type, const TypeLibraryPointer& typeLibrary);
        virtual std::map<std::string, Members::MemberPointer> GetAllMembers() const;
        virtual std::list<std::string> GetUnsortedMembers() const;
    private:
        std::list<TypePointer> m_baseTypes;
    };

void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraryPointer typeLibrary, TypeWithBases**);

}}}}