#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/ControlElement.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    //Marks all control elements as added to prevent re-adding them later.
    class LayoutMarkAddedElementsHandler : public LayoutHandler<LayoutMarkAddedElementsHandler, ControlElement> {
    public:
        using BaseClass::ModifyLayout;
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<ControlElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
    };
}}}}