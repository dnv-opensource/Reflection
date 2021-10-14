#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/NameAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    If member is a property, this handler will add the PropertyAspect to the node.
    */
    class ControlNodeNameAttributeHandler : public IControlNodeAttributeHandler<Attributes::NameAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::NameAttribute& nameAttribute, bool priorState = false) override;
    };
}}}}