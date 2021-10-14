//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeBindingAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	bool ControlNodeBindingAttributeHandler::OnInitialize(ControlNode& node, const Attributes::BindingAttribute& attribute, bool priorState)
	{
		if (!node.GetDecoratedTypeInfo().IsValid() || node.GetDecoratedTypeInfo() == Types::TypeId<void>())
		{
			node.InitializeType(attribute.GetReturnType(), priorState);
		}
		return priorState;
	}

}}}}

