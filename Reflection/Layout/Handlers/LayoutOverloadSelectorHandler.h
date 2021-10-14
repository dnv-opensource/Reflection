#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/Selector.h"
#include "Reflection/Layout/OverloadSelector.h"
#include "Reflection/Layout/TabControl.h"
#include "Reflection/Layout/DynamicArea.h"
#include "Reflection/Layout/Collapsable.h"
#include "Reflection/Layout/SelectorLookup.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutOverloadSelectorHandler : public LayoutHandler<LayoutOverloadSelectorHandler,OverloadSelector, Selector, SelectorLookup, TabControl, Collapsable, DynamicArea>
    {
    public:
        using BaseClass::ModifyLayout;
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<OverloadSelector>& overloadSelector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<Selector>& selector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<SelectorLookup>& selector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<TabControl>& selector, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<Collapsable>& collapsable, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<DynamicArea>& dynamicArea, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
    };
}}}}