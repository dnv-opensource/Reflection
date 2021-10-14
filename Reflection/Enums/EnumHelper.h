#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Members/IMemberFwd.h"
#include "Formatting/IStringFormatter.h"
#include <set>
#include <list>
#include "Formatting/FormattingService.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Objects { class Object; }}}}

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {
    class REFLECTION_IMPORT_EXPORT EnumHelper 
    {
    public:
        //Requires lookup in service provider to find the typelibrary.
        EnumHelper(const std::type_index& enumType);
        EnumHelper(const TypeLibraries::TypePointer& type);
        EnumHelper(TypeLibraries::TypeLibraryPointer typeLibrary, const std::type_index& enumType);
        TypeLibraries::TypePointer GetType() const;
        std::string GetName() const;
        std::string GetEnumValueName(int enumValue);
        std::string GetFormattedEnumValueName(const std::string enumName);
        std::string GetFormattedEnumValueName(int enumValue);
        const DNVS::MoFa::Formatting::IStringFormatter& GetFormatter(int enumValue);
        void SetFormatter(const DNVS::MoFa::Formatting::IStringFormatter& formatter);
        Members::MemberPointer GetMember(const Objects::Object& o) const;
        Members::MemberPointer GetMember(int enumValue) const;
        
        std::list<Members::MemberPointer> GetAllEnumMembers() const;
        std::set<long> GetAllEnumValues() const;
        std::pair<int, bool> GetEnumValue(const std::string& member) const;
        std::pair<int, bool> GetEnumValue(const Members::MemberPointer& member) const;
        std::list<Variants::Variant> GetAllReflectedEnumValues() const;
        std::string ToString(int enumValue, const DNVS::MoFa::Formatting::FormattingService& service) const;
        std::string ToString(const Members::MemberPointer& member, const DNVS::MoFa::Formatting::FormattingService& service) const;
        std::pair<std::string, Members::MemberPointer> GetNameAndMember(int enumValue) const;
        int TryGetAlias(Members::MemberPointer member, int value) const;
    private:
        std::string GetNameOfMember(const Members::MemberPointer& member) const;
        TypeLibraries::TypePointer m_type;
    };
}}}}
