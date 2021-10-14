#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/Grid.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    //Handle Headers of grids
    class LayoutHeaderAspectHandler : public LayoutHandler<LayoutHeaderAspectHandler, Grid> {
    public:
        using BaseClass::ModifyLayout;
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<Grid>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
    };
}}}}