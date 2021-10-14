//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutSymmetryAttributeHandler.h"
#include "Reflection/Attributes/SymmetryAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutSymmetryAttributeHandler::ModifyLayout(const std::shared_ptr<GroupBox>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (auto attribute = controlNode->TryGetAttribute<Attributes::SymmetryAttribute>())
        {
            auto child = controlNode->GetChild(attribute->GetSymmetryName());
            if (child)
            {
                auto enabler = std::make_shared<CheckBox>();
                enabler->SetControlNode(child);
                layoutElement->SetEnabler(enabler);
            }
        }
        return layoutElement;
    }

}}}}
