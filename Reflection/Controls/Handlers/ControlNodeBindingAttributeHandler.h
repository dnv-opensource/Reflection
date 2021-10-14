#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Attributes/BindingAttribute.h"
#include "IControlNodeAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler initializes the caption of the node.
    */
    class ControlNodeBindingAttributeHandler : public IControlNodeAttributeHandler<Attributes::BindingAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::BindingAttribute& attribute, bool priorState = false);
    };
}}}}