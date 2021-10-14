#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>
#include <set>
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    class REFLECTION_IMPORT_EXPORT ConstructorsFromTypeService
    {
    public:
        ConstructorsFromTypeService(const TypeLibraries::TypeLibraryPointer& typeLibrary);
        std::set<Members::MemberPointer> CollectAllConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo, bool allowAll);
        std::set<Members::MemberPointer> CollectAllSerializationConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo);
        static std::shared_ptr<ConstructorsFromTypeService> GetService(const TypeLibraries::TypeLibraryPointer& typeLibrary);
    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        std::map<std::pair<Types::DecoratedTypeInfo, bool>, std::set<Members::MemberPointer> > m_constructorMembers;
        std::map<Types::DecoratedTypeInfo, std::set<Members::MemberPointer> > m_serializationMembers;
    };
}}}}