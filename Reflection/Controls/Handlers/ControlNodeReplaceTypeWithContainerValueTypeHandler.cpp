//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeReplaceTypeWithContainerValueTypeHandler.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "../ControlNodeManager.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	void ControlNodeReplaceTypeWithContainerValueTypeHandler::OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& type)
	{
		auto attribute = node.TryGetAttribute<Attributes::ContainerAttribute>();
		if (attribute)
			node.InitializeType(attribute->GetValueType(), false);
	}

}}}}

