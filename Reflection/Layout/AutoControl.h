#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "TypedLayoutElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class AutoControl : public TypedLayoutElement<AutoControl,ControlElement>
    {
    public:
        AutoControl(const AutoControl&) = default;
        AutoControl(const std::shared_ptr<Controls::ControlNode>& controlNode) : TypedLayoutElement<AutoControl, ControlElement>(controlNode) {}
        explicit AutoControl(const std::string& name, const std::string& caption = "__USENAME__") : TypedLayoutElement<AutoControl, ControlElement>(name, caption) {}
		template<typename AttributeT, typename... Args>
		AutoControl& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
    };

	template<typename T>
	AutoControl Auto(const std::string& name, const std::string& caption = "__USENAME__")
	{
		AutoControl control(name, caption);
		control.SetTypeInfo(Types::TypeId<T>());
		return control;
	}

    template<typename StringT, typename LayoutT>
    constexpr bool CanConvertToAutoControl = std::is_base_of_v<LayoutT, AutoControl> && std::is_convertible_v<StringT, std::string>;
}}}}
