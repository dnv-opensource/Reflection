#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Attributes/CaptionAttribute.h"
#include "IControlNodeAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler initializes the caption of the node.
    */
    class ControlNodeCaptionAttributeHandler : public IControlNodeAttributeHandler<Attributes::CaptionAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::CaptionAttribute& attribute, bool priorState = false);
    };
}}}}