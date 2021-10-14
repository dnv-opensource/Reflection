//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutGridHandler.h"
#include "Reflection/Controls/Aspects/ContainerAspect.h"
#include "LayoutControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutGridHandler::ModifyLayout(const std::shared_ptr<Grid> grid, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
		if (grid->GetControlNode() == nullptr)
		{
			if (auto actualNode = grid->TryGetMatchingControlNode(controlNode))
			{
				grid->SetControlNode(actualNode);
			}
		}
		return grid;
    }

}}}}

