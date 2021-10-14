#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    //Embeds layoutElement in a group box.
    class LayoutGroupAttributeHandler : public ILayoutHandler {
    public:
        std::shared_ptr<ILayoutElement> ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context) override;
    };
}}}}