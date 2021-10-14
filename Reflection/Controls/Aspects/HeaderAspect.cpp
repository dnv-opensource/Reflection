//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "HeaderAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	HeaderAspect::HeaderAspect(const Layout::HorizontalElements& header)
		: m_header(std::make_shared<Layout::HorizontalElements>(header))
	{

	}

	const Attributes::AttributeCollection& HeaderAspect::GetAttributeCollection() const
	{
		return Attributes::AttributeCollection::GetEmptyCollection();
	}

	std::string HeaderAspect::ToString() const
	{
		return "HeaderAspect(" + Layout::ToString(*m_header) + ")";
	}

	const std::shared_ptr<Layout::HorizontalElements>& HeaderAspect::GetHeader() const
	{
		return m_header;
	}

	bool HeaderAspect::operator==(const HeaderAspect& other) const
	{
		return *m_header == *other.m_header;
	}

}}}}

