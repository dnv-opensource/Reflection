//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutOverloadSelectorHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutOverloadSelectorHandler::ModifyLayout(const std::shared_ptr<OverloadSelector>& overloadSelector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto actualNode = overloadSelector->TryGetMatchingControlNode(controlNode))
        {
            overloadSelector->SetControlNode(actualNode);
            overloadSelector->SetChild(ModifyLayout(overloadSelector->GetChild(), actualNode, context));
        }
        else
        {
            overloadSelector->SetChild(ModifyLayout(overloadSelector->GetChild(), controlNode, context));
        }
        return overloadSelector;
    }

    std::shared_ptr<ILayoutElement> LayoutOverloadSelectorHandler::ModifyLayout(const std::shared_ptr<Selector>& selector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto controlElement = std::dynamic_pointer_cast<ControlElement>(selector->GetChild()))
        {
            if (controlElement->GetControlNode())
                return selector;
            auto actualNode = controlElement->TryGetMatchingControlNode(controlNode);
            if (!actualNode)
                actualNode = controlNode;
            std::string caption = controlElement->GetCaption();
            if (caption == "__USENAME__")
                caption = controlElement->GetName();
            auto selectorNode = actualNode->GetName() == "__SELECTOR__"?actualNode:actualNode->GetChild("__SELECTOR__");
            if (selectorNode)
            {
                controlElement->SetControlNode(selectorNode);
                controlElement->SetCaption(caption);
            }
        }
        return selector;
    }

    std::shared_ptr<ILayoutElement> LayoutOverloadSelectorHandler::ModifyLayout(const std::shared_ptr<TabControl>& selector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        auto child = controlNode->GetChild("__SELECTOR__");
        if (child)
            selector->SetControlNode(child);
        for (auto& child : *selector)
            child = ModifyLayout(child, controlNode, context);
        return selector;
    }

    std::shared_ptr<ILayoutElement> LayoutOverloadSelectorHandler::ModifyLayout(const std::shared_ptr<Collapsable>& collapsable, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {        
        if (auto child = controlNode->GetChild("__SELECTOR__"))
        {
            if (auto expanded = child->GetChild("Expanded"))
                collapsable->SetControlNode(expanded);
            collapsable->SetChild(ModifyLayout(collapsable->GetChild(), controlNode, context));
        }
        return collapsable;
    }

    std::shared_ptr<ILayoutElement> LayoutOverloadSelectorHandler::ModifyLayout(const std::shared_ptr<DynamicArea>& dynamicArea, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto child = controlNode->GetChild("__SELECTOR__"))
            dynamicArea->SetControlNode(child);
        return dynamicArea;
    }

    std::shared_ptr<ILayoutElement> LayoutOverloadSelectorHandler::ModifyLayout(const std::shared_ptr<SelectorLookup>& selector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (selector->GetControlNode())
            return selector;
        auto actualNode = selector->TryGetMatchingControlNode(controlNode);
        if (!actualNode)
            actualNode = controlNode;
        std::string caption = selector->GetCaption();
        auto selectorNode = actualNode->GetName() == "__SELECTOR__" ? actualNode : actualNode->GetChild("__SELECTOR__");
        if (selectorNode)
        {
            selector->SetControlNode(selectorNode);
            selector->SetName(actualNode->GetName());
            selector->SetCaption(caption);
        }
        return selector;
    }

}}}}

