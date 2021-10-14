//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TabGroupCreator.h"
#include "Reflection/Attributes/NameAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    TabGroupCreator::TabGroupCreator(const Attributes::TabAttribute& tabAttribute)
        : m_tabAttribute(tabAttribute)
        , m_tabControl(std::make_shared<TabControl>())
    {
    }

    void TabGroupCreator::AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        if (controlNode->TryGetAttribute<Attributes::NameAttribute>() && controlNode->GetParent() == nullptr)
            AddTabElement(layoutElement, Attributes::TabAttribute(Attributes::TabPosition::AboveTab));
        else if (auto tabAttribute = controlNode->TryGetAttribute<Attributes::TabAttribute>())
            AddTabElement(layoutElement, *tabAttribute);
        else
            AddTabElement(layoutElement, m_tabAttribute);
    }

    std::shared_ptr<ILayoutElement> TabGroupCreator::GetGroup() const
    {
        if (m_above.empty() && m_below.empty())
            return m_tabControl;
        else
        {
            auto verticalElements = std::make_shared<VerticalElements>();
            for (const LayoutElementPointer& element : m_above)
                verticalElements->PushBack(element);
            verticalElements->PushBack(m_tabControl);
            for (const LayoutElementPointer& element : m_below)
                verticalElements->PushBack(element);
            return verticalElements;
        }
    }

    void TabGroupCreator::AddTabElement(const std::shared_ptr<ILayoutElement>& layoutElement, const Attributes::TabAttribute& tabAttribute)
    {
        switch (tabAttribute.GetTabPosition())
        {
        case Attributes::TabPosition::AboveTab:
            return m_above.push_back(layoutElement);
        case Attributes::TabPosition::BelowTab:
            return m_below.push_back(layoutElement);
        default:
            GetOrCreateTab(tabAttribute.GetName())->PushBack(layoutElement);
        }
    }

    std::shared_ptr<VerticalElements> TabGroupCreator::GetOrCreateTab(const std::string& tabName)
    {
        std::shared_ptr<VerticalElements>& verticalElements = m_tabs[tabName];
        if (!verticalElements)
        {
            verticalElements = std::make_shared<VerticalElements>();
            m_tabControl->PushBackTab(EmbeddedElement(tabName, verticalElements));
        }
        return verticalElements;
    }

}}}}

