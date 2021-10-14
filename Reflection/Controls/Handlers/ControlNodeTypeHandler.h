#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeHandler.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler is used when initializing a ControlNode with a type.
    It will search that type for a constructor and populate the node based on the constructor it found.
    It will only be used if node is a root node with no other aspects than a TypeAspect.
    */
    class ControlNodeTypeHandler : public IControlNodeHandler
    {
    public:
        void OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& decoratedTypeInfo) override;
        bool OnInitialize(ControlNode& node, bool priorState = false) override;
    };
}}}}