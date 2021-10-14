//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeWithGlobalMemberRegistration.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberWithArguments.h"
#include "Reflection/Classes/Class.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    TypeWithGlobalMemberRegistration::TypeWithGlobalMemberRegistration( TypePointer type, TypePointer globalType )
        : TypeWithForwarding(type)
        ,   m_globalType(globalType)
    {
    }

    Members::MemberPointer TypeWithGlobalMemberRegistration::AddMember(const std::string& name, Members::MemberPointer member)
    {
        Members::MemberPointer returnType = TypeWithForwarding::AddMember(name, member);
        if(member->GetMemberType() == Members::MemberType::TypeConstructor)
        {
            std::string constructorName = name;
            if(constructorName == Members::ConstructorString) 
                constructorName = GetName();
            m_globalType->AddMember(constructorName, member);
        }
        return returnType;
    }

    void DoReflect(TypeLibraryPointer typeLibrary, TypeWithGlobalMemberRegistration**)
    {
        using namespace Classes;
        Class<TypeWithGlobalMemberRegistration, Public<TypeWithForwarding>, TypePointer> cls(typeLibrary, "TypeWithForwarding");
    }

}}}}