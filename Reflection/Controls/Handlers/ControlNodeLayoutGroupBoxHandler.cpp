//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutGroupBoxHandler.h"
#include "Reflection/Controls/Aspects/GroupAspect.h"
#include "Reflection/Attributes/GroupAttribute.h"
#include "GroupAspectAdder.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutGroupBoxHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::GroupBox>& element, bool priorState)
    {
        if (!node.TryGetAttribute<Attributes::GroupAttribute>() && !element->GetName().empty())
        {
            node.AddAspect<GroupAspect>(element->GetName());
            GroupAspectAdder(element->GetName()).OnInitialize(node, element, priorState);
        }
        return priorState;
    }

}}}}


