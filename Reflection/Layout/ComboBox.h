#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ComboBox : public TypedLayoutElement<ComboBox, ControlElement>
    {
    public:
        ComboBox(const ComboBox&) = default;
        ComboBox(ComboBox&&) = default;
        ComboBox& operator=(const ComboBox&) = default;
        ComboBox& operator=(ComboBox&&) = default;

        ComboBox(const AutoControl& other) : TypedLayoutElement<ComboBox, ControlElement>(other) {}
        ComboBox(const std::shared_ptr<Controls::ControlNode>& controlNode) : TypedLayoutElement<ComboBox, ControlElement>(controlNode) {}
        explicit ComboBox(const std::string& name = "", const std::string& caption = "__USENAME__") : TypedLayoutElement<ComboBox, ControlElement>(name, caption) {}
		template<typename AttributeT, typename... Args>
		ComboBox& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
	};
}}}}
