#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/RecordAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will check if member has the RecordAttribute and initialize the node with children according to the specifications on the RecordAttribute.
    It will also add the RecordAccessorAspect.
    */
    class ControlNodeRecordAttributeHandler : public IControlNodeHandler
    {
    public:
        bool OnInitializeMember(ControlNode& node, const Members::MemberPointer& member);
    };
}}}}