#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Attributes/TabAttribute.h"
#include "ILayoutElementGroupCreator.h"
#include "Reflection/Layout/TabControl.h"
#include "Reflection/Layout/EmbeddedElement.h"
#include "Reflection/Layout/VerticalElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class TabGroupCreator : public ILayoutElementGroupCreator
    {
    public:
        TabGroupCreator(const Attributes::TabAttribute& tabAttribute);
        void AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode) override;
        std::shared_ptr<ILayoutElement> GetGroup() const override;
    private:
        void AddTabElement(const std::shared_ptr<ILayoutElement>& layoutElement, const Attributes::TabAttribute& tabAttribute);
        std::shared_ptr<VerticalElements> GetOrCreateTab(const std::string& tabName);
        const Attributes::TabAttribute& m_tabAttribute;
        std::shared_ptr<TabControl> m_tabControl;
        std::map<std::string, std::shared_ptr<VerticalElements>> m_tabs;
        std::list<LayoutElementPointer> m_above;
        std::list<LayoutElementPointer> m_below;
    };
}}}}