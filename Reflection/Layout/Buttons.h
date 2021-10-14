#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"
#include "Reflection/Variants/VariantService.h"
#include <vector>
#include "NamedItem.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Buttons : public TypedLayoutElement<Buttons, ControlElement>
    {
    public:
        Buttons(const Buttons&) = default;
        Buttons(Buttons&&) = default;
        Buttons& operator=(const Buttons&) = default;
        Buttons& operator=(Buttons&&) = default;

        explicit Buttons(const std::string& name, size_t horizontalRows = 1)
            : TypedLayoutElement<Buttons, ControlElement>(name, "__USENAME__")
            , m_horizontalRows(horizontalRows)
        {}
        explicit Buttons(const std::string& name, const std::string& caption, size_t horizontalRows = 1)
            : TypedLayoutElement<Buttons, ControlElement>(name, caption)
            , m_horizontalRows(horizontalRows)
        {}
        explicit Buttons(size_t horizontalRows = 1)
            : TypedLayoutElement<Buttons, ControlElement>("", "__USENAME__")
            , m_horizontalRows(horizontalRows)
        {}
        explicit Buttons(size_t horizontalRows, const std::vector<NamedItem>& items)
            : TypedLayoutElement<Buttons, ControlElement>("", "__USENAME__")
            , m_horizontalRows(horizontalRows)
            , m_items(items)
        {}
        size_t GetHorizontalRows() const { return m_horizontalRows; }
        const std::vector<NamedItem>& GetItems() const { return m_items; }
		template<typename AttributeT, typename... Args>
		Buttons& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
    private:
        size_t m_horizontalRows;
        std::vector<NamedItem> m_items;
    };
}}}}
