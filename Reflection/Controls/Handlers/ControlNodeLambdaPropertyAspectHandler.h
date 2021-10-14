#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will parse the node for a IncludeInViewAttribute. If one is found, it will add the properties specified in this attribute as children to the node.
    */
    class ControlNodeLambdaPropertyAspectHandler : public IControlNodeHandler
    {
    public:
        bool OnInitialize(ControlNode& node, bool priorState = false) override;
    };
}}}}