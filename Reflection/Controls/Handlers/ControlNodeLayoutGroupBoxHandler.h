#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutAttributeHandler.h"
#include "Reflection/Layout/GroupBox.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Handle GroupBox from the LayoutAttribute. Adds GroupAspect.
    */
    class ControlNodeLayoutGroupBoxHandler : public ControlNodeLayoutAttributeHandler<ControlNodeLayoutGroupBoxHandler, Layout::GroupBox>
    {
    public:
        using BaseClass::OnInitialize;
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::GroupBox>& element, bool priorState = false);
    };
}}}}