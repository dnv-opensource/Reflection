#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Layout/HorizontalElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Defines the header of a table or a grid. Used when accessing subelements of a container.
    //Then we must ignore this layout.
    class REFLECTION_IMPORT_EXPORT HeaderAspect : public IControlAspectT<HeaderAspect>
    {
    public:
		HeaderAspect(const Layout::HorizontalElements& header);
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const HeaderAspect& other) const override;
        std::string ToString() const override;
		const std::shared_ptr<Layout::HorizontalElements>& GetHeader() const;
	private:
		std::shared_ptr<Layout::HorizontalElements> m_header;
    };

}}}}