#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionOfElements.h"
#include "TypedLayoutElement.h"
#include <xutility>
#include "EmbeddedElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    class TabControl : public TypedLayoutElement<TabControl, CollectionOfElements>
    {
    public:
        TabControl(const TabControl& other) = default;
        TabControl(TabControl&& other) = default;
        TabControl& operator=(const TabControl& other) = default;
        TabControl& operator=(TabControl&& other) = default;

        template<typename... TabsT>
        explicit TabControl(TabsT&&... args)
        {
            PushBackTab(std::forward<TabsT>(args)...);
        }
        void PushBackTab()
        {}
        template<typename... TabsT>
        void PushBackTab(EmbeddedElement&& tab, TabsT&&... remaining)
        {
            PushBack(std::move(tab));
            std::dynamic_pointer_cast<EmbeddedElement>(m_elements.back())->SetProxy(true);
            PushBackTab(std::forward<EmbeddedElement>(remaining)...);
        }
        std::shared_ptr<EmbeddedElement> GetTabAt(size_t index) const
        {
            if (index < size())
            {
                auto it = begin();
                std::advance(it, index);
                return std::dynamic_pointer_cast<EmbeddedElement>(*it);
            }
            return nullptr;
        }
        void SetControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode) { m_controlNode = controlNode; }
        std::shared_ptr<Controls::ControlNode> GetControlNode() const { return m_controlNode; }
    private:
        std::shared_ptr<Controls::ControlNode> m_controlNode;
    };
}}}}
