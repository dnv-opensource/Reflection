//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeNameAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeNameAttributeHandler::OnInitialize(ControlNode& node, const Attributes::NameAttribute& nameAttribute, bool priorState)
    {
        if (node.GetParent())
            return priorState;
        node.SetName("__this__");
        node.SetCaption(nameAttribute.GetCaption());
        return priorState;
    }

}}}}

