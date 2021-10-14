#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The ExpandMemberAttribute is used to control whether a member should be expanded or not.
    This overrides the default behavior of expanding members.
    */
    enum class ExpandOptions
    {
        NoExpansion = 0,
        CanExpand = 1,
        ExpandedByDefault = 2,
    };
    inline ExpandOptions operator|(ExpandOptions lhs, ExpandOptions rhs)
    {
        return ExpandOptions(int(lhs) | int(rhs));
    }
    inline ExpandOptions operator&(ExpandOptions lhs, ExpandOptions rhs)
    {
        return ExpandOptions(int(lhs) & int(rhs));
    }
    inline bool Has(ExpandOptions lhs, ExpandOptions rhs)
    {
        return (lhs&rhs) == rhs;
    }

    class ExpandMemberAttribute {
    public:
        ExpandMemberAttribute(ExpandOptions option)
            : m_option(option)
        {}
        bool CanExpand() const { return Has(m_option, ExpandOptions::CanExpand); }
        bool ExpandedByDefault() const { return Has(m_option, ExpandOptions::ExpandedByDefault); }
    private:
        ExpandOptions m_option;
    };
}}}}

