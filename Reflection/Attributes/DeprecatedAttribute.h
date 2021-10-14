#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The DeprecatedAttribute is used in reflection in order to indicate that a member is no longer to be used.
    Example:
    Class<StiffenerSpacingMemOpt> cls(typeLibrary, "StiffenerSpacingMemOpt");
    cls.SetGet("StiffenerSpacing",...)
       .AddAttribute<DeprecatedAttribute>("StiffenerSpacingMember");

    The argument to DeprecatedAttribute is the new name of the member (if any)    
    */
    class DeprecatedAttribute {
    public:
        DeprecatedAttribute(const std::string& alternativeName = "") 
            : m_alternativeName(alternativeName) {}
        const std::string& GetAlternativeName() const { return m_alternativeName; }
    private:
        std::string m_alternativeName;
    };
}}}}