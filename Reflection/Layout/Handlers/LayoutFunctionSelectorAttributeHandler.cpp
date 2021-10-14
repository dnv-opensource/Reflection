//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutFunctionSelectorAttributeHandler.h"
#include "Reflection/Attributes/FunctionSelectorAttribute.h"
#include "Reflection/Layout/VerticalElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutFunctionSelectorAttributeHandler::ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        auto attribute = controlNode->TryGetAttribute<Attributes::FunctionSelectorAttribute>();
        if (!attribute || !std::dynamic_pointer_cast<AutoControl>(layoutElement))
            return layoutElement;
        return std::make_shared<VerticalElements>(layoutElement, OverloadSelector(attribute->GetOverloadSelector()));
    }

}}}}

