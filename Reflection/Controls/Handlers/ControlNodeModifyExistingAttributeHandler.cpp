//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeModifyExistingAttributeHandler.h"
#include "../Aspects/DisabledAspect.h"
#include "../Aspects/ConstructorAspect.h"
#include "../Aspects/FunctionArgumentAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeModifyExistingAttributeHandler::OnInitialize(ControlNode& node, const Attributes::ModifyExistingAttribute& attribute, bool priorState)
    {
        if(attribute.GetDisableSelectionOfModified())
            node.AddAspect<DisabledAspect>();
        if (node.TryGetAspect<ConstructorAspect>())
        {
            for (const auto& child : node.GetChildren())
            {
                if (child->TryGetAspect<FunctionArgumentAspect>() && !child->SupportAssign())
                {
                    child->AddAspect<DisabledAspect>();
                    if (auto selector = child->GetChild("__SELECTOR__"))
                        selector->AddAspect<DisabledAspect>();
                }
            }
        }
        return priorState;
    }

}}}}

