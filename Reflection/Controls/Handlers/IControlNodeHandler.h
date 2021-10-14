#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class ControlNode;
    //Interface for handling aspects of a control node.
    class IControlNodeHandler
    {
    public:
        virtual ~IControlNodeHandler() = 0{}
        virtual bool OnInitialize(ControlNode& node, bool priorState = false) { return false; }
        virtual bool OnInitializeMember(ControlNode& node, const Members::MemberPointer& member) { return false; }
        virtual void OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& decoratedTypeInfo) {}
    };

}}}}