#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHandler.h"
#include "Reflection/Layout/AutoControl.h"
#include "Reflection/Layout/Grid.h"
#include "Reflection/Attributes/RecordAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutRecordAttributeHandler : public LayoutHandler<LayoutRecordAttributeHandler, AutoControl, Grid> {
    public:
        using BaseClass::ModifyLayout;
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<AutoControl>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<Grid>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context);
        virtual std::shared_ptr<ILayoutElementGroupCreator> CreateGroupCreator(const std::shared_ptr<Controls::ControlNode>& controlNode) override;
    private:
        const Attributes::RecordAttribute* TryGetRecordAttributeForGrid(const std::shared_ptr<Controls::ControlNode>& controlNode) const;
    };
}}}}