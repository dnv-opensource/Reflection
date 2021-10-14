//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutNestedLayoutAttributeHandler.h"
#include "Contexts/ILayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutNestedLayoutAttributeHandler::ModifyLayout(const std::shared_ptr<ControlElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto matchingControlNode = layoutElement->TryGetMatchingControlNode(controlNode))
        {
            if (matchingControlNode != controlNode)
                return context.GetManager()->ModifyLayout(layoutElement, matchingControlNode, context);
        }
        return layoutElement;
    }

    std::shared_ptr<ILayoutElement> LayoutNestedLayoutAttributeHandler::ModifyLayout(const std::shared_ptr<SelectorLookup>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        return layoutElement;
    }

}}}}

