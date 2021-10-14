//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutEliminateAliasNodeHandler.h"
#include "Reflection/Layout/AutoControl.h"
#include "Reflection/Controls/Aspects/IsAliasAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutEliminateAliasNodeHandler::ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (std::dynamic_pointer_cast<AutoControl>(layoutElement) && controlNode->TryGetAspect<Controls::IsAliasAspect>())
            return nullptr;
        return layoutElement;
    }

}}}}

