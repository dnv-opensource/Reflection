//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeConstructorSelectorAttributeHandler.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeConstructorSelectorAttributeHandler::OnInitialize(ControlNode& node, const Attributes::ConstructorSelectorAttribute& attribute, bool priorState)
    {
        auto childNode = node.GetChild("__SELECTOR__");
        if (childNode)
        {
            auto selector = childNode->TryGetAspect<ConstructorSelectorAspect>();
            if (selector)
                childNode->AddAspect<ConstructorSelectorAspect>(selector->GetDecoratedTypeInfo(), attribute.GetMembers(node.GetTypeLibrary(), selector->GetConstructors()));
        }
        return priorState;
    }

}}}}

