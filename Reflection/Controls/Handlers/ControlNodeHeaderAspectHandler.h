#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeHandler.h"
#include "Reflection/Layout/ControlElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This handler will parse the node that has a header aspect.
    */
    class ControlNodeHeaderAspectHandler : public IControlNodeHandler
    {
    public:
        bool OnInitialize(ControlNode& node, bool priorState = false) override;
	private:
        bool CanSetName(ControlNode& node, const Layout::ControlElement& element) const;
		void Connect(const std::shared_ptr<ControlNode>& node, const std::shared_ptr<Layout::ControlElement>& element);
    };
}}}}