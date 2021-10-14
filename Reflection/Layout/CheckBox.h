#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class CheckBox : public TypedLayoutElement<CheckBox, ControlElement>
    {
    public:
        CheckBox(const CheckBox&) = default;
        CheckBox(CheckBox&&) = default;
        CheckBox& operator=(const CheckBox&) = default;
        CheckBox& operator=(CheckBox&&) = default;

        CheckBox(const AutoControl& other) : TypedLayoutElement<CheckBox, ControlElement>(other) {}
        CheckBox(const std::shared_ptr<Controls::ControlNode>& controlNode) : TypedLayoutElement<CheckBox, ControlElement>(controlNode) {}
        explicit CheckBox(const std::string& name="", const std::string& caption = "__USENAME__") : TypedLayoutElement<CheckBox, ControlElement>(name, caption) {}
		template<typename AttributeT, typename... Args>
		CheckBox& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
	};
}}}}
