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
    class RadioButtons : public TypedLayoutElement<RadioButtons, ControlElement>
    {
    public:
        RadioButtons(const RadioButtons&) = default;
        RadioButtons(RadioButtons&&) = default;
        RadioButtons& operator=(const RadioButtons&) = default;
        RadioButtons& operator=(RadioButtons&&) = default;

        RadioButtons(const AutoControl& other)
            : TypedLayoutElement<RadioButtons, ControlElement>(other)
            , m_verticalColumns(1)
        {}
        RadioButtons(const std::shared_ptr<Controls::ControlNode>& controlNode)
            : TypedLayoutElement<RadioButtons, ControlElement>(controlNode) 
            , m_verticalColumns(1)
        {}
        explicit RadioButtons(const std::string& name, size_t verticalColumns = 1) 
            : TypedLayoutElement<RadioButtons, ControlElement>(name, "__USENAME__")
            , m_verticalColumns(verticalColumns)
        {}
        explicit RadioButtons(const std::string& name, const std::string& caption, size_t verticalColumns = 1)
            : TypedLayoutElement<RadioButtons, ControlElement>(name, caption)
            , m_verticalColumns(verticalColumns)
        {}
        explicit RadioButtons(size_t verticalColumns = 1)
            : TypedLayoutElement<RadioButtons, ControlElement>("", "__USENAME__")
            , m_verticalColumns(verticalColumns)
        {}
        explicit RadioButtons(size_t verticalColumns, const std::vector<NamedItem>& items)
            : TypedLayoutElement<RadioButtons, ControlElement>("", "__USENAME__")
            , m_verticalColumns(verticalColumns)
            , m_items(items)
        {}
        size_t GetVerticalColumns() const { return m_verticalColumns; }
        const std::vector<NamedItem>& GetItems() const { return m_items; }
		template<typename AttributeT, typename... Args>
		RadioButtons& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
    private:
        size_t m_verticalColumns;
        std::vector<NamedItem> m_items;
    };
}}}}
