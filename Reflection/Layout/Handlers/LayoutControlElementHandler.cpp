//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutControlElementHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutControlElementHandler::ModifyLayout(const std::shared_ptr<ControlElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (layoutElement->GetControlNode() == nullptr)
        {
            if (auto actualNode = layoutElement->TryGetMatchingControlNode(controlNode))
            {
                layoutElement->SetControlNode(actualNode);
            }            
        }
        return layoutElement;
    }

}}}}

