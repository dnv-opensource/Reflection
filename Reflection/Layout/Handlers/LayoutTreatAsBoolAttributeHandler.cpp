//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutTreatAsBoolAttributeHandler.h"
#include "Reflection/Attributes/TreatAsBoolAttribute.h"
#include "Reflection/Layout/CheckBox.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    std::shared_ptr<ILayoutElement> LayoutTreatAsBoolAttributeHandler::ModifyLayout(const std::shared_ptr<AutoControl>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto actualNode = layoutElement->TryGetMatchingControlNode(controlNode))
        {
            if (actualNode->TryGetAttribute<Attributes::TreatAsBoolAttribute>())
                return std::make_shared<CheckBox>(actualNode);
        }
        return layoutElement;
    }
}}}}




