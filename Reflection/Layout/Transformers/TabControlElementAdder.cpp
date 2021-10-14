//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TabControlElementAdder.h"
#include "Reflection/Attributes/TabAttribute.h"
#include "Reflection/Layout/Visitors/HasMemberVisitor.h"
#include "Reflection/Attributes/AttributeCollectionService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    TabControlElementAdder::TabControlElementAdder(const std::string& defaultTab)
        : m_defaultTab(defaultTab)
        , m_tabControl(std::make_shared<TabControl>())
    {
        GetOrCreateTab(defaultTab);
    }

    void TabControlElementAdder::AddElement(const LayoutElementPointer& element, const Attributes::AttributeCollection& attributeCollection)
    {
        std::string tabName = m_defaultTab;
        const Attributes::TabAttribute* tabAttribute = Attributes::GetPointerToAttributeOrNull<Attributes::TabAttribute>(attributeCollection);
        if (tabAttribute)
        {
            tabName = tabAttribute->GetName();
            switch(tabAttribute->GetTabPosition())
            {
                case Attributes::TabPosition::AboveTab:
                    m_above.push_back(element);
                    return;
                case Attributes::TabPosition::BelowTab:
                    m_below.push_back(element);
                    return;
            }
        }
        GetOrCreateTab(tabName)->PushBack(element);
    }

    bool TabControlElementAdder::HasMember(const Members::MemberPointer& member) const
    {
        HasMemberVisitor visitor(member);
        m_tabControl->Accept(visitor);
        return visitor.GetResult();
    }

    LayoutElementPointer TabControlElementAdder::GetLayoutElement() const
    {
        if(m_above.empty() && m_below.empty())
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

    std::shared_ptr<VerticalElements> TabControlElementAdder::GetOrCreateTab(const std::string& tabName)
    {
        std::shared_ptr<VerticalElements>& verticalElements = m_verticalElements[tabName];
        if (!verticalElements)
        {
            verticalElements = std::make_shared<VerticalElements>();
            m_tabControl->PushBackTab(EmbeddedElement(tabName, verticalElements));
        }
        return verticalElements;
    }

}}}}

