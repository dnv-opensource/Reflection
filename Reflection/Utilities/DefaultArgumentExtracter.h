#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include <map>
#include "TypeUtilities/CompareCaseLess.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes { class  DefaultAttribute; } } } }
namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    class REFLECTION_IMPORT_EXPORT DefaultArgumentExtracter
    {
    public:
        DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member);
        DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& constructor, const std::string& argumentName);
        DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& constructor, size_t argumentIndex);
        DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, const Members::MemberPointer& constructor, size_t argumentIndex);
        DefaultArgumentExtracter(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, const Members::MemberPointer& constructor, const std::string& argumentName);
        Variants::Variant TryGetDefaultValue(const std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess>& additionalArguments = {}) const;
        static size_t GetIndexOfNamedConstructorArgument(const Members::MemberPointer& constructor, const std::string& argumentName);
        Variants::Variant TryGetDefaultValue(const Attributes::DefaultAttribute& defaultAttribute, const std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess>& additionalArguments) const;
    private:
        Variants::Variant TryGetDefaultValueFromMember(const std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess>& additionalArguments) const;
        Variants::Variant TryGetDefaultValueFromConstructorArgument() const;
        Members::MemberPointer m_member;
        Members::MemberPointer m_constructor;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        std::size_t m_argumentIndex;
    };
}}}}