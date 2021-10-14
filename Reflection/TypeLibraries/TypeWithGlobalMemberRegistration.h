#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Members/GlobalType.h"
#include "TypeWithForwarding.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {    
    /// <summary>
    /// A type helper that registers all constructors also to the global namespace.
    /// </summary>
    /// <seealso cref="TypeWithForwarding" />
    class REFLECTION_IMPORT_EXPORT TypeWithGlobalMemberRegistration : public TypeWithForwarding
    {
    public:
        TypeWithGlobalMemberRegistration(TypePointer type, TypePointer globalType);
        virtual Members::MemberPointer AddMember( const std::string& name, Members::MemberPointer member ) override;
    private:
        TypePointer m_globalType;
    };

    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraryPointer typeLibrary, TypeWithGlobalMemberRegistration**);

}}}}