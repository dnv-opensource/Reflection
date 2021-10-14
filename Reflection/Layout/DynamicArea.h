#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedLayoutElement.h"
#include "ILayoutElement.h"
#include "Reflection/Controls/ControlNode.h"
#include "EmbeddedElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class DynamicArea : public TypedLayoutElement<DynamicArea, ILayoutElement>
    {
    public:
        DynamicArea(const DynamicArea& other) = default;
        DynamicArea(DynamicArea&& other) = default;
        DynamicArea& operator=(const DynamicArea& other) = default;
        DynamicArea& operator=(DynamicArea&& other) = default;

        DynamicArea() = default;
        void SetControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode) { m_controlNode = controlNode; }
        std::shared_ptr<Controls::ControlNode> GetControlNode() const { return m_controlNode; }

		//This constructor is used during testing to create a baseline.
		explicit DynamicArea(const EmbeddedElement& embedded) 
			: m_embedded(std::make_shared<EmbeddedElement>(embedded)) 
		{}
		const std::shared_ptr<EmbeddedElement>& GetEmbedded() const { return m_embedded; }
    private:
        std::shared_ptr<Controls::ControlNode> m_controlNode;
		std::shared_ptr<EmbeddedElement> m_embedded;
    };
}}}}