#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IElementAdder.h"
#include "Reflection/Layout/VerticalElements.h"
#include "Reflection/Layout/TabControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class TabControlElementAdder : public IElementAdder {
    public:
        TabControlElementAdder(const std::string& defaultTab);
        virtual void AddElement(const LayoutElementPointer& element, const Attributes::AttributeCollection& attributeCollection);
        virtual bool HasMember(const Members::MemberPointer& member) const;
        virtual LayoutElementPointer GetLayoutElement() const;
    private:
        std::shared_ptr<VerticalElements> GetOrCreateTab(const std::string& tabName);
        std::list<LayoutElementPointer> m_above;
        std::list<LayoutElementPointer> m_below;
        std::shared_ptr<TabControl> m_tabControl;
        std::string m_defaultTab;
        std::map<std::string, std::shared_ptr<VerticalElements>> m_verticalElements;
    };
}}}}