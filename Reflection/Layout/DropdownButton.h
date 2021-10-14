#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "TypeUtilities/Tribool.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class DropdownButton : public TypedLayoutElement<DropdownButton, ControlElement>
    {
    public:
        DropdownButton(const DropdownButton&) = default;
        DropdownButton(DropdownButton&&) = default;
        DropdownButton& operator=(const DropdownButton&) = default;
        DropdownButton& operator=(DropdownButton&&) = default;

        DropdownButton(const std::shared_ptr<Controls::ControlNode>& controlNode) : TypedLayoutElement<DropdownButton, ControlElement>(controlNode) {}
        explicit DropdownButton(const std::string& name = "", const std::string& caption = "__USENAME__", TypeUtilities::Tribool defaultExpanded = TypeUtilities::Indeterminate) 
			: TypedLayoutElement<DropdownButton, ControlElement>(name, caption) 
			, m_defaultExpanded(defaultExpanded)
		{}
		TypeUtilities::Tribool IsDefaultExpanded() const { return m_defaultExpanded; }
	private:
		TypeUtilities::Tribool m_defaultExpanded;
	};
}}}}
