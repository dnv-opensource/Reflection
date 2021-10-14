//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeWithBases.h"
#include "ITypeLibrary.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {    

    TypeWithBases::TypeWithBases(const TypePointer& type, const TypeLibraryPointer& typeLibrary) : TypeWithForwarding(type)
    {
        for (const auto& baseTypeIndex : type->GetBaseTypes())
        {
            auto baseType = typeLibrary->LookupType(baseTypeIndex);
            if (baseType)
                m_baseTypes.push_back(std::make_shared<TypeWithBases>(baseType, typeLibrary));
        }
    }

    std::map<std::string, Members::MemberPointer> TypeWithBases::GetAllMembers() const
    {
        std::map<std::string, Members::MemberPointer> allMembers = TypeWithForwarding::GetAllMembers();
        for (const auto& baseType : m_baseTypes)
        {
            for (const auto& memberPair : baseType->GetAllMembers())
                allMembers.insert(memberPair);
        }
        return allMembers;
    }

    std::list<std::string> TypeWithBases::GetUnsortedMembers() const
    {
        std::list<std::string> allMembers = TypeWithForwarding::GetUnsortedMembers();
        for (const auto& baseType : m_baseTypes)
        {
            for (std::string member : baseType->GetUnsortedMembers())
                allMembers.push_back(member);
        }
        return allMembers;
    }
}}}}