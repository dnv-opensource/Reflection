#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/GroupBox.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    //Embeds layoutElement in a group box.
    class LayoutGroupEnablerAttributeHandler : public LayoutHandler<LayoutGroupEnablerAttributeHandler, GroupBox, CheckBox> {
    public:
        using BaseClass::ModifyLayout;
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<GroupBox>& groupBox, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<CheckBox>& checkBox, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
    };
}}}}