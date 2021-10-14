//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeGroupEnablerAttributeHandler.h"
#include "Reflection/Controls/Aspects/GroupEnablerAspect.h"
#include "Reflection/Controls/Aspects/GroupAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeGroupEnablerAttributeHandler::OnInitialize(ControlNode& node, const Attributes::GroupEnablerAttribute& attribute, bool priorState)
    {
        auto groupAspect = node.TryGetAspect<GroupAspect>();
        if (!groupAspect)
            throw std::runtime_error("You have defined a GroupEnablerAttribute for your member \"" + node.GetName() + "\", but you also need to define a group, either using the GroupAttribute or by using the GroupBox in a LayoutAttribute.");

        if (attribute.HasFunction())
        {
            auto parent = node.GetParent();
            if (parent)
            {
                auto checkBox = std::make_shared<ControlNode>(parent, Types::TypeId<bool>());
                checkBox->SetName(groupAspect->GetGroupName());
                parent->AddChild(checkBox);
                checkBox->AddAspect<Controls::GroupEnablerAspect>(attribute, groupAspect->GetGroupName());
            }
        }
        else
            node.SetCaption(groupAspect->GetGroupName());
        return priorState;
    }

}}}}

