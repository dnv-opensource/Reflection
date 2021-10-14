//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutSelectorHandler.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutSelectorHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::Selector>& element, bool priorState)
    {
        if (auto controlElement = std::dynamic_pointer_cast<Layout::ControlElement>(element->GetChild()))
        {
            auto actualNode = LookupRelatedNode(node, controlElement);
            if (!actualNode)
                actualNode = &node;
            auto selector = actualNode->GetChild("__SELECTOR__");
            if (!selector)
                return priorState;            
            if (controlElement->GetCaption() == "__SELECTOR__")
                selector->SetCaption("");
            else
                selector->SetCaption(controlElement->GetCaption());
        }
        return priorState;
    }

}}}}

