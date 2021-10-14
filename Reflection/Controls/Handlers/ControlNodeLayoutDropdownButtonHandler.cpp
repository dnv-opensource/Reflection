//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutDropdownButtonHandler.h"
#include "Reflection/Controls/Aspects/ExpandActionAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutDropdownButtonHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::DropdownButton>& element, bool priorState)
    {
        auto child = node.GetChild("__SELECTOR__");
        if (child)
        {
            auto expanded = std::make_shared<ControlNode>(child.get(), Types::TypeId<bool>(), "Expanded");
            child->AddChild(expanded);
            expanded->AddAspect<ExpandActionAspect>(false);
        }
        return priorState;
    }

}}}}

