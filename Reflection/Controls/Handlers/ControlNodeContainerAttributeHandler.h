#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/ContainerAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Handles containers
    */
    class ControlNodeContainerAttributeHandler : public IControlNodeAttributeHandler<Attributes::ContainerAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::ContainerAttribute& attribute, bool priorState = false) override;
    };
}}}}