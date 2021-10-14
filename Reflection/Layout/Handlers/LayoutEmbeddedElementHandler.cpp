//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutEmbeddedElementHandler.h"
#include "Reflection/Layout/AutoControl.h"
#include "Reflection/Layout/VerticalElements.h"
#include "Contexts/ILayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutEmbeddedElementHandler::ModifyLayout(const std::shared_ptr<EmbeddedElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if(auto actualNode = layoutElement->TryGetMatchingControlNode(controlNode))
        {
            layoutElement->SetControlNode(actualNode);
            if (layoutElement->GetChild())
                ModifyLayout(layoutElement->GetChild(), actualNode, context);
            else
            {
                auto group = std::make_shared<VerticalElements>();
                for (const auto& child : controlNode->GetChildren())
                {
                    auto layoutOfChild = context.GetManager()->ModifyLayout(std::make_shared<AutoControl>(child), child, context);
                    if (layoutOfChild)
                        group->PushBack(layoutOfChild);
                }
                layoutElement->SetChild(group);
            }
        }
        return layoutElement;
    }

}}}}

