#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Reflection/Config.h"
#include "Reflection/Objects/Object.h"
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    /*
    This class will, for any type, return the list of constructors that will return an object of that type.
    What constitutes a valid constructor changes for different contexts, so we have two methods.
    The first, CollectAllConstructorMembersOfThisAndDerivedClasses, is used when autogenerating GUI.
    The second, CollectAllSerializationConstructorMembersOfThisAndDerivedClassesm, is used when autogenerating serialization.
    */
    class REFLECTION_IMPORT_EXPORT ConstructorsFromTypeExtracter
    {
    public:
        ConstructorsFromTypeExtracter(TypeLibraries::TypeLibraryPointer typeLibrary);
        ~ConstructorsFromTypeExtracter();
        std::set<Members::MemberPointer> CollectAllConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo, bool allowAll) const;
        std::set<Members::MemberPointer> CollectAllSerializationConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
        Members::MemberPointer GetBestMemberFromType(const std::set<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
        Members::MemberPointer GetBestMemberFromItem(const Objects::Object& object, const std::set<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
        bool IsConstructorMember(Members::MemberPointer overload, const Types::DecoratedTypeInfo& decoratedTypeInfo, bool allowAll) const;
        bool IsConstructorMember(Members::MemberPointer overload, bool allowAll) const;
    private:
        std::list<Members::MemberPointer> GetNonDeprecatedMembers(const std::list<Members::MemberPointer>& members) const;
        std::list<Members::MemberPointer> GetMembersOfConcreteType(const std::list<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
        std::list<Members::MemberPointer> GetLegalMembers(const std::set<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
        bool IsSerializationConstructorMember(Members::MemberPointer overload, const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        std::list<Members::MemberPointer> GetDeducibleMembers(const std::list<Members::MemberPointer>& members) const;
        std::list<Members::MemberPointer> GetMembersWithMoreThanZeroArguments(const std::list<Members::MemberPointer>& members) const;
        std::list<Members::MemberPointer> GetDefaultMembers(const std::list<Members::MemberPointer>& members) const;
        std::list<Members::MemberPointer> GetMembersWithSameReturnType(const std::list<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const;
    };
}}}}

