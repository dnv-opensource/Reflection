//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CellLayoutManager.h"
#include "LayoutLayoutAttributeForCellsHandler.h"
#include "LayoutBoolHandler.h"
#include "LayoutTreatAsBoolAttributeHandler.h"
#include "LayoutEnumerableAttributeHandler.h"
#include "LayoutFallbackHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

	CellLayoutManager::CellLayoutManager()
		: LayoutManager(false)
	{
		AddHandler(new LayoutLayoutAttributeForCellsHandler);
		AddHandler(new LayoutBoolHandler);
		AddHandler(new LayoutTreatAsBoolAttributeHandler);
		AddHandler(new LayoutEnumerableAttributeHandler);
		AddHandler(new LayoutFallbackHandler);
	}

}}}}

