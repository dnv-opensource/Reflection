#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/EmbeddedElement.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    //Remove EmbeddedElement that has children. They are not needed in order to represent the layout.
    class LayoutEliminateStaticEmbeddedElementsHandler : public LayoutHandler<LayoutEliminateStaticEmbeddedElementsHandler, EmbeddedElement> {
    public:
        using BaseClass::ModifyLayout;
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<EmbeddedElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
    };
}}}}