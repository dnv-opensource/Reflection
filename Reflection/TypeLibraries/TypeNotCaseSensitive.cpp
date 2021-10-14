//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeNotCaseSensitive.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberWithArguments.h"
#include "Reflection/Classes/Class.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    TypeNotCaseSensitive::TypeNotCaseSensitive(TypePointer type)
        : TypeWithForwarding(type)
    {

    }

    Members::MemberPointer TypeNotCaseSensitive::AddMember(const std::string& name, Members::MemberPointer member)
    {
        m_caseLessSortedNames.insert(name);
        return TypeWithForwarding::AddMember(name, member);
    }

    Members::MemberWithArgumentsPointer TypeNotCaseSensitive::PrepareInvoke(const std::string& name, const std::vector<Variants::Variant>& args, Members::MemberType type /*= Members::MemberType::TypeAll*/) const
    {
        return TypeWithForwarding::PrepareInvoke(GetNameWithCorrectCase(name), args, type);
    }

    Members::MemberPointer TypeNotCaseSensitive::Lookup(const std::string& name) const
    {
        return TypeWithForwarding::Lookup(GetNameWithCorrectCase(name));
    }

    Members::MemberPointer TypeNotCaseSensitive::Lookup(const std::string& name, const Variants::Variant& object) const
    {
        return TypeWithForwarding::Lookup(GetNameWithCorrectCase(name), object);
    }

    const std::string& TypeNotCaseSensitive::GetNameWithCorrectCase(const std::string& name) const
    {
        auto it = m_caseLessSortedNames.find(name);
        if (it == m_caseLessSortedNames.end())
            return name;
        else
            return *it;
    }

    void DoReflect(TypeLibraryPointer typeLibrary, TypeNotCaseSensitive**)
    {
        using namespace Classes;
        Class<TypeNotCaseSensitive, Public<TypeWithForwarding>, TypePointer> cls(typeLibrary, "TypeWithForwarding");
    }

}}}}

