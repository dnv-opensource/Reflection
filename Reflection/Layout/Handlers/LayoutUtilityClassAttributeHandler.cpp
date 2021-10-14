//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutUtilityClassAttributeHandler.h"
#include "Reflection/Attributes/UtilityClassAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutUtilityClassAttributeHandler::ModifyLayout(const std::shared_ptr<AutoControl>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto attribute = controlNode->TryGetAttribute<Attributes::UtilityClassAttribute>())
        {
            if (!layoutElement->GetControlNode())
                layoutElement->SetControlNode(controlNode);
        }
        return layoutElement;
    }

}}}}

