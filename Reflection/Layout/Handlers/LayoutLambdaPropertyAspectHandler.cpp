//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutLambdaPropertyAspectHandler.h"
#include "Reflection/Controls/Aspects/LambdaPropertyAspect.h"
#include "Contexts/ILayoutControlContext.h"
#include "GroupCreators/ILayoutElementGroupCreator.h"
#include "ILayoutHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutLambdaPropertyAspectHandler::ModifyLayout(const std::shared_ptr<AutoControl>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (controlNode->TryGetAspect<Controls::LambdaPropertyAspect>() && controlNode->GetName().empty())
        {
            if (controlNode->GetChildren().empty())
                return nullptr;
            auto groupCreator = context.GetManager()->CreateGroupCreator(controlNode);
            if (!groupCreator)
                return nullptr;
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

