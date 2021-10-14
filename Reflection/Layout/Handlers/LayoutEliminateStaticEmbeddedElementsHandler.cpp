//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutEliminateStaticEmbeddedElementsHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutEliminateStaticEmbeddedElementsHandler::ModifyLayout(const std::shared_ptr<EmbeddedElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        layoutElement->SetChild(ModifyLayout(layoutElement->GetChild(), controlNode, context));
        if (layoutElement->GetControlNode() && layoutElement->GetChild() && !layoutElement->IsProxy())
            return layoutElement->GetChild();
        else
            return layoutElement;
    }

}}}}

