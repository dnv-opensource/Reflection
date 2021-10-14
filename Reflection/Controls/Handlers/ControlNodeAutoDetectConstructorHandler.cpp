//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeAutoDetectConstructorHandler.h"
#include "ControlNodeHandlerUtilities.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/IsFunctionalNodeAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeAutoDetectConstructorHandler::OnInitialize(ControlNode& node, bool priorState)
    {
        if (node.GetChildren().empty() || node.TryGetAspect<ConstructorAspect>() || node.TryGetAspect<IsFunctionalNodeAspect>() || node.GetChild("__SELECTOR__"))
            return priorState;
        std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess> members;
        for (const auto& child : node.GetChildren())
            members.emplace(child->GetName(), child);
        TryAddConstructor(node, members);
        return priorState;
    }

}}}}

