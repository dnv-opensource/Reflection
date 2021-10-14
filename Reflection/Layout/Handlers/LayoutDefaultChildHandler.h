#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/AutoControl.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutDefaultChildHandler : public ILayoutHandler
    {
    public:
        std::shared_ptr<ILayoutElement> ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
    };
}}}}