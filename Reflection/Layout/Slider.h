#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"
#include "Header\Header.h"
#include "TypeUtilities/Tribool.h"
#include "TypedLayoutElement.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
                
    class Slider : public TypedLayoutElement<Slider, ControlElement>
    {
    public:
        Slider(const Slider&) = default;
        Slider(Slider&&) = default;
        Slider& operator=(const Slider&) = default;
        Slider& operator=(Slider&&) = default;

        Slider(const AutoControl& other) : TypedLayoutElement<Slider, ControlElement>(other), m_header(true) {}

        explicit Slider(const std::string& name = "", const std::string& caption = "__USENAME__")
            : TypedLayoutElement<Slider, ControlElement>(name, caption) 
            , m_header(true) 
        {}
        explicit Slider(const std::string& name, Header&& header, const std::string& caption = "__USENAME__")
            : TypedLayoutElement<Slider, ControlElement>(name, caption) 
            , m_header(std::move(header))
        {}
        const Header& GetHeader() const { return m_header; }
		template<typename AttributeT, typename... Args>
		Slider& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
	private:
        Header m_header;
    };
} } } }
