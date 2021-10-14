//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutOverloadSelectorHandler.h"
#include "ControlNodeHandlerUtilities.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/IsFunctionalNodeAspect.h"
#include "Reflection/Controls/Aspects/IgnoreLayoutAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutOverloadSelectorHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::OverloadSelector>& element, bool priorState)
    {
        if (node.TryGetAspect<ConstructorAspect>() || node.TryGetAspect<IsFunctionalNodeAspect>())
        {
            node.AddAspect<IgnoreLayoutAspect>();
            return priorState;
        }
        TryAddConstructorSelector(node);
        return priorState;
    }

}}}}

