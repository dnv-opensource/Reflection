//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeExpandMemberAttributeHandler.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/IsFunctionalNodeAspect.h"
#include "Reflection/Controls/Aspects/ExpandActionAspect.h"
#include "ControlNodeHandlerUtilities.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    
    bool ControlNodeExpandMemberAttributeHandler::OnInitialize(ControlNode& node, const Attributes::ExpandMemberAttribute& attribute, bool priorState)
    {
        if(node.TryGetAspect<ConstructorAspect>() || node.TryGetAspect<IsFunctionalNodeAspect>())
            return priorState;
        //Layout attribute overrides ExpandMemberAttribute.
        if (!attribute.CanExpand())
            return priorState;
        else
        {
            std::shared_ptr<ControlNode> expanded;
            auto child = node.GetChild("__SELECTOR__");
            if (child)
                expanded = child->GetChild("Expanded");
            else
            {
                TryAddConstructorSelector(node);
                child = node.GetChild("__SELECTOR__");
                if (child)
                {
                    expanded = std::make_shared<ControlNode>(child.get(), Types::TypeId<bool>(), "Expanded");
                    child->AddChild(expanded);
                }
            }
            if(expanded)
                expanded->AddAspect<ExpandActionAspect>(attribute.ExpandedByDefault());
        }
        return priorState;
    }

}}}}

