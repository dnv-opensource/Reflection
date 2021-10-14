#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Formatting/FormattingService.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    /*
    This item will take any member, and format it as an invocation.
    */
    class REFLECTION_IMPORT_EXPORT MemberItem
    {
    public:
        MemberItem() {}
        MemberItem(Members::MemberPointer member, TypeLibraries::TypeLibraryPointer typeLibrary);
        bool operator==(const MemberItem& other) const;
        Members::MemberPointer GetMember() const;
        std::string GetOrGenerateName(size_t index) const;
        //Check that all arguments are named and that they have matching properties (get+set) on the return type.
        bool AreConstructorArgumentBoundToObjectProperties() const;
        bool AreConstructorArgumentsNamed() const;
        bool IsConstructor() const;
        bool IsUtilityClassFunction() const;
        Types::DecoratedTypeInfo GetTypeInfo() const;
        TypeLibraries::TypeLibraryPointer GetTypeLibrary() const;
    private:
        Members::MemberPointer m_member;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };

    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MemberItem**);
    std::string REFLECTION_IMPORT_EXPORT ToString(const MemberItem& item, const DNVS::MoFa::Formatting::FormattingService& formattingService);

    class REFLECTION_IMPORT_EXPORT LexicalMemberItemSorter {
    public:
        LexicalMemberItemSorter(const DNVS::MoFa::Formatting::FormattingService& service = DNVS::MoFa::Formatting::FormattingService());
        bool operator()(const MemberItem& lhs, const MemberItem& rhs) const;
    private:
        DNVS::MoFa::Formatting::FormattingService m_formattingService;
    };
}}}}
