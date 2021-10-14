//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutMarkAddedElementsHandler.h"
#include "Contexts/ILayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutMarkAddedElementsHandler::ModifyLayout(const std::shared_ptr<ControlElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if(layoutElement && layoutElement->GetControlNode())
            context.AddLayout(layoutElement->GetControlNode());
        return layoutElement;
    }

}}}}

