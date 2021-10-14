//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutRemoveVerticalElementFromGroupBoxHandler.h"
#include "Reflection/Layout/VerticalElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutRemoveVerticalElementFromGroupBoxHandler::ModifyLayout(const std::shared_ptr<GroupBox>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        std::list<std::shared_ptr<ILayoutElement>> children;
        for (auto& child : *layoutElement)
        {
            child = ModifyLayout(child, controlNode, context);
            if (auto nestedVerticalElement = std::dynamic_pointer_cast<VerticalElements>(child))
            {
                for (const auto& nestedChild : *nestedVerticalElement)
                    children.push_back(nestedChild);
            }
            else
                children.push_back(child);
        }
        layoutElement->Clear();
        for (const auto& child : children)
            layoutElement->PushBack(child);
        return layoutElement;
    }

}}}}

