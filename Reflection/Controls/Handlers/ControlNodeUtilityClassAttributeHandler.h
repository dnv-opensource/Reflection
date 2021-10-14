#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/UtilityClassAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will ensure all members of the selected type will be included as sub nodes.
    */
    class ControlNodeUtilityClassAttributeHandler : public IControlNodeAttributeHandler<Attributes::UtilityClassAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::UtilityClassAttribute& attribute, bool priorState = false);
    };
}}}}