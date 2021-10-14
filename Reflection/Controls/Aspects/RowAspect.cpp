//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RowAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	RowAspect::RowAspect(const std::shared_ptr<ControlNode>& rowNode)
		: m_rowNode(rowNode)
	{

	}

	const Attributes::AttributeCollection& RowAspect::GetAttributeCollection() const
	{
		return Attributes::AttributeCollection::GetEmptyCollection();
	}

	std::string RowAspect::ToString() const
	{
		return "RowAspect(...)";
	}

	const std::shared_ptr<ControlNode>& RowAspect::GetRowNode() const
	{
		return m_rowNode;
	}

	bool RowAspect::operator==(const RowAspect& other) const
	{
		return true;
	}

}}}}

