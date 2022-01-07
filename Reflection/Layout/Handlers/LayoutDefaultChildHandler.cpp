//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutDefaultChildHandler.h"
#include "Reflection/Layout/VerticalElements.h"
#include "Contexts/ILayoutControlContext.h"
#include "GroupCreators/ILayoutElementGroupCreator.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "../../Attributes/UtilityClassAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutDefaultChildHandler::ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (controlNode != context.GetRootNode())
            return layoutElement;

        auto autoControl = std::dynamic_pointer_cast<AutoControl>(layoutElement);
        if (autoControl)
        {
            if (controlNode->GetChildren().empty() && !controlNode->TryGetAspect<Controls::ConstructorAspect>())
                return layoutElement;
            auto groupCreator = context.GetManager()->CreateGroupCreator(controlNode);
            if (!groupCreator)
                return layoutElement;
            if (autoControl->GetControlNode() && !autoControl->GetControlNode()->TryGetAttribute<Attributes::UtilityClassAttribute>())
                groupCreator->AddLayoutElement(layoutElement, autoControl->GetControlNode());
            for (const auto& child : controlNode->GetChildren())
            {
                auto layoutOfChild = context.GetManager()->ModifyLayout(std::make_shared<AutoControl>(child->GetName(), child->GetCaption()), child, context);
                if (layoutOfChild)
                    groupCreator->AddLayoutElement(layoutOfChild, child);
            }
            return groupCreator->GetGroup();
        }
        return layoutElement;
    }

}}}}

