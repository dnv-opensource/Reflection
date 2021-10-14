#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include <functional>
#include <set>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Allows customization of which constructors shall be listed for a certain property.
    */
    class ConstructorSelectorAttribute {
    public:
        typedef std::function<std::set<Members::MemberPointer>(const TypeLibraries::TypeLibraryPointer&, const std::set<Members::MemberPointer>& fullSetOfConstructors)> Selector;
        ConstructorSelectorAttribute(const Selector& selector)
            : m_selector(selector)
        {}
        std::set<Members::MemberPointer> GetMembers(const TypeLibraries::TypeLibraryPointer& typeLibrary, const std::set<Members::MemberPointer>& fullSetOfConstructors) const
        {
            return m_selector(typeLibrary, fullSetOfConstructors);
        }
    private:
        Selector m_selector;
    };
}}}}
