//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeCaptionAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeCaptionAttributeHandler::OnInitialize(ControlNode& node, const Attributes::CaptionAttribute& attribute, bool priorState)
    {
        node.SetCaption(attribute.GetCaption());
        //Don't terminate initialization even if we have a caption attribute.
        return priorState;
    }


}}}}

