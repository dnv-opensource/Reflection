//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutHeaderAspectHandler.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/Aspects/HeaderAspect.h"
#include "CellLayoutManager.h"
#include "Contexts/LayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

	std::shared_ptr<ILayoutElement> LayoutHeaderAspectHandler::ModifyLayout(const std::shared_ptr<Grid>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
	{
		if (auto aspect = controlNode->TryGetAspect<Controls::HeaderAspect>())
		{
			auto gridManager = std::make_shared<CellLayoutManager>();
			HorizontalElements header;
			LayoutControlContext context(gridManager, controlNode);
			for (auto it = aspect->GetHeader()->begin(); it != aspect->GetHeader()->end(); ++it)
				header.PushBack(gridManager->ModifyLayout(*it, nullptr, context));
			layoutElement->SetHeader(header);
		}
		return layoutElement;
	}

}}}}

