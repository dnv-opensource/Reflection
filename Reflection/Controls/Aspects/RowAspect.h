#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
	class ControlNode;
    /*
    This aspect is assigned to the row node in order to store the row node template.
    */
    class REFLECTION_IMPORT_EXPORT RowAspect : public IControlAspectT<RowAspect>
    {
    public:
		RowAspect(const std::shared_ptr<ControlNode>& rowNode);
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const RowAspect& other) const override;
        std::string ToString() const override;
		const std::shared_ptr<ControlNode>& GetRowNode() const;
	private:
		std::shared_ptr<ControlNode> m_rowNode;
    };

}}}}