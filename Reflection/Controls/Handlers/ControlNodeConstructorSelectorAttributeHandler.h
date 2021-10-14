#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/ConstructorSelectorAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler takes the ConstructorSelectorAttribute into account. This attribute will take a collection of constructors and return a subset of them for use in overload resolution.
    Example: JoinedCurveOffset contains two overload nodes: Lhs and Rhs. These overload nodes can contain for example 
    - ConstantCurveOffset and 
    - LinearVaryingCurveOffset, 
    - but NOT NoCurveOffset, so we need to filter it away for Lhs and Rhs.
    */
    class ControlNodeConstructorSelectorAttributeHandler : public IControlNodeAttributeHandler<Attributes::ConstructorSelectorAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::ConstructorSelectorAttribute& attribute, bool priorState = false) override;
    };
}}}}