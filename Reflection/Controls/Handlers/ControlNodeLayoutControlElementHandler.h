#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutAttributeHandler.h"
#include "Reflection/Layout/ControlElement.h"
#include "Reflection/Layout/Grid.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    Handle ControlElements from the LayoutAttribute. Allows user to override the type of any control, such as ComboBox instead of RadioButtons for enums.
    */
    class ControlNodeLayoutControlElementHandler : public ControlNodeLayoutAttributeHandler<ControlNodeLayoutControlElementHandler, Layout::Grid, Layout::ControlElement>
    {
    public:
        using BaseClass::OnInitialize;
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element, bool priorState = false);
		bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::Grid>& element, bool priorState = false);
    private:
        void SetData(ControlNode& node, const Layout::ControlElement& element);
		void SetGridData(ControlNode& node, const Layout::Grid& element);
    };
}}}}