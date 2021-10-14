#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/TupleAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Inserts a boolean node that will control whether two nodes in your GUI should depend on each other.
    */
    class ControlNodeTupleAttributeHandler : public IControlNodeAttributeHandler<Attributes::TupleAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::TupleAttribute& attribute, bool priorState = false) override;
    };
}}}}