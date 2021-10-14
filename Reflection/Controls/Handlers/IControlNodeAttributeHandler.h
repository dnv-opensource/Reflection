#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Controls/ControlNode.h"
#include "IControlNodeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Interface for handling attributes on a control node.
    template<typename AttributeT>
    class IControlNodeAttributeHandler : public IControlNodeHandler
    {
    public:
        bool OnInitialize(ControlNode& node, bool priorState = false) override
        {
            if (!node.GetTypeLibrary())
                return priorState;
            for (const auto& aspect : node.GetAspects())
            {
                if (aspect->GetAttributeCollection().HasAttribute<AttributeT>())
                    return OnInitialize(node, aspect->GetAttributeCollection().GetAttribute<AttributeT>(), priorState);
            }
            return priorState;
        }
        virtual bool OnInitialize(ControlNode& node, const AttributeT& attribute, bool priorState = false) = 0;
    };

}}}}