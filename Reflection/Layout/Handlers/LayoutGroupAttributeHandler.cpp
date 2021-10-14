//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutGroupAttributeHandler.h"
#include "Reflection/Attributes/GroupAttribute.h"
#include "Reflection/Layout/GroupBox.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutGroupAttributeHandler::ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (!layoutElement)
            return nullptr;

        if (auto groupAttribute = controlNode->TryGetAttribute<Attributes::GroupAttribute>())
        {
            auto groupBox = std::make_shared<GroupBox>(groupAttribute->GetGroup(), layoutElement);
            groupBox->AddDocumentation(groupAttribute->GetDocumentation());
            return groupBox;
        }            
        return layoutElement;
    }

}}}}

