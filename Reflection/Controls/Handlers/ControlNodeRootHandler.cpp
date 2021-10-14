//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeRootHandler.h"
#include "Reflection/Controls/Aspects/RootAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeRootHandler::OnInitialize(ControlNode& node, bool priorState)
    {
        if (!node.GetParent())
            node.AddAspect<RootAspect>();
        return priorState;
    }

}}}}

