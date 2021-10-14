//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeContainerAttributeHandler.h"
#include "Reflection/Controls/Aspects/ContainerAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeContainerAttributeHandler::OnInitialize(ControlNode& node, const Attributes::ContainerAttribute& attribute, bool priorState)
    {
        node.AddAspect<ContainerAspect>(node.GetDecoratedTypeInfo(), attribute);
        return priorState;
    }

}}}}

