#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class TextBox : public TypedLayoutElement<TextBox, ControlElement>
    {
    public:
        TextBox(const TextBox&) = default;
        TextBox(TextBox&&) = default;
        TextBox& operator=(const TextBox&) = default;
        TextBox& operator=(TextBox&&) = default;

        TextBox(const AutoControl& other) : TypedLayoutElement<TextBox, ControlElement>(other) {}
        TextBox(const std::shared_ptr<Controls::ControlNode>& controlNode) : TypedLayoutElement<TextBox, ControlElement>(controlNode) {}
        explicit TextBox(const std::string& name="", const std::string& caption = "__USENAME__") : TypedLayoutElement<TextBox, ControlElement>(name, caption) {}
		template<typename AttributeT, typename... Args>
		TextBox& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
	};
}}}}
