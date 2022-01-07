//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeGroupAttributeHandler.h"
#include "Reflection/Controls/Aspects/GroupAspect.h"
#include "../../Attributes/LayoutAttribute.h"
#include "GroupAspectAdder.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeGroupAttributeHandler::OnInitialize(ControlNode& node, const Attributes::GroupAttribute& attribute, bool priorState)
    {
        node.AddAspect<GroupAspect>(attribute.GetGroup());
        if (auto layout = node.TryGetAttribute<Attributes::LayoutAttribute>())
            GroupAspectAdder(attribute.GetGroup()).OnInitialize(node, *layout, priorState);
        return priorState;
    }

}}}}

