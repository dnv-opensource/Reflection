#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/ExpandMemberAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will parse the ExpandMemberAttribute.
    The ExpandMemberAttribute is used either for specifying that this node acts as a overload selector, or that the child elements of this node should be embedded into this layout.
    Alternatively, it can be used for disabling member expansion altogether for a node.
    */
    class ControlNodeExpandMemberAttributeHandler : public IControlNodeAttributeHandler<Attributes::ExpandMemberAttribute>
    {
    public:
        bool OnInitialize(ControlNode& node, const Attributes::ExpandMemberAttribute& attribute, bool priorState = false);
    };
}}}}