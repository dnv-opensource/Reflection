#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/ModifyExistingAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will parse the ModifyAttribute.
    The ModifyAttribute will disable all read only fields.
    */
    class ControlNodeModifyExistingAttributeHandler : public IControlNodeAttributeHandler<Attributes::ModifyExistingAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::ModifyExistingAttribute& attribute, bool priorState = false);
    };
}}}}