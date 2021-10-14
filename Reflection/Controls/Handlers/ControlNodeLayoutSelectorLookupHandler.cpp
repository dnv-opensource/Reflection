//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutSelectorLookupHandler.h"
#include "Reflection/Controls/Aspects/AliasAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutSelectorLookupHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::SelectorLookup>& element, bool priorState)
    {
        if (node.GetName() != element->GetName())
        {
            auto actualNode = node.LookupRelatedNode(element->GetName());
            if (actualNode)
            {
                auto nodeChild = node.GetChild("__SELECTOR__");
                auto lookupChild = actualNode->GetChild("__SELECTOR__");
                if (nodeChild && lookupChild && nodeChild != lookupChild)
                {
                    nodeChild->AddAspect<AliasAspect>(lookupChild);
                }
            }
        }
        return priorState;
    }

}}}}

