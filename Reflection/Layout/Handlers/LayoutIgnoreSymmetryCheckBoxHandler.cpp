//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutIgnoreSymmetryCheckBoxHandler.h"
#include "Reflection/Controls/Aspects/SymmetryAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutIgnoreSymmetryCheckBoxHandler::ModifyLayout(const std::shared_ptr<AutoControl>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (controlNode->TryGetAspect<Reflection::Controls::SymmetryAspect>())
            return nullptr;
        else
            return layoutElement;
    }

}}}}

