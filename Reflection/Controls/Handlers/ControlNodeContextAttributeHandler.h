#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/ContextAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Reduces the number of relevant members to include as child nodes.
    */
    class ControlNodeContextAttributeHandler : public IControlNodeAttributeHandler<Attributes::ContextAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::ContextAttribute& attribute, bool priorState = false) override;
    };
}}}}