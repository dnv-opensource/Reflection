//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutLayoutAttributeForCellsHandler.h"
#include "Reflection/Layout/CheckBox.h"
#include "Reflection/Layout/TextBox.h"
#include "Reflection/Layout/ComboBox.h"
#include "Reflection/Controls/Aspects/IgnoreLayoutAspect.h"
#include "Reflection/Attributes/LayoutAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

	std::shared_ptr<ILayoutElement> LayoutLayoutAttributeForCellsHandler::ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
	{
		if (!layoutElement)
			return nullptr;
		if (!controlNode || controlNode->TryGetAspect<Controls::IgnoreLayoutAspect>())
			return layoutElement;
		if (auto layoutAttribute = controlNode->TryGetAttribute<Attributes::LayoutAttribute>())
		{
			if (layoutAttribute->GetLayout())
			{
				if (std::dynamic_pointer_cast<CheckBox>(layoutAttribute->GetLayout()))
					return layoutAttribute->GetLayout()->Clone();
				else if (std::dynamic_pointer_cast<TextBox>(layoutAttribute->GetLayout()))
					return layoutAttribute->GetLayout()->Clone();
				else if (std::dynamic_pointer_cast<ComboBox>(layoutAttribute->GetLayout()))
					return layoutAttribute->GetLayout()->Clone();
			}
		}
		return layoutElement;
	}

}}}}

