#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Handles the root node, the one without any parents. Add the RootAspect to it.
    */
    class ControlNodeRootHandler : public IControlNodeHandler
    {
    public:
        virtual bool OnInitialize(ControlNode& node, bool priorState = false);
    };
}}}}