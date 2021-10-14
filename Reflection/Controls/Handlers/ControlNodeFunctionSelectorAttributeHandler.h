#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/FunctionSelectorAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will parse the FunctionSelectorAttribute.
    The FunctionSelectorAttribute is used for specifying a set of functions to be showed in a dialog.
    The user can select one of these functions to be executed.
    */
    class ControlNodeFunctionSelectorAttributeHandler : public IControlNodeAttributeHandler<Attributes::FunctionSelectorAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::FunctionSelectorAttribute& attribute, bool priorState = false);
    };
}}}}