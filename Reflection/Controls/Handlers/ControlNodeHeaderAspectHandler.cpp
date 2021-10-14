//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeHeaderAspectHandler.h"
#include "Reflection/Controls/Aspects/HeaderAspect.h"
#include "Reflection/Controls/Aspects/RowAspect.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "ControlNodeReplaceTypeWithContainerValueTypeHandler.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "../Aspects/FunctionArgumentAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	bool ControlNodeHeaderAspectHandler::OnInitialize(ControlNode& node, bool priorState)
	{
		if (const HeaderAspect* aspect = node.TryGetAspect<HeaderAspect>())
		{
			std::shared_ptr<Layout::HorizontalElements> header = aspect->GetHeader();
			auto containerAttribute = node.TryGetAttribute<Attributes::ContainerAttribute>();
			if (!containerAttribute)
				return priorState;
			auto manager = std::make_shared<ControlNodeManager>(node.GetTypeLibrary());
			manager->AddHandler(new ControlNodeReplaceTypeWithContainerValueTypeHandler);
			auto rowNode = std::make_shared<ControlNode>(manager, containerAttribute->GetValueType());			
			auto rowIndex = std::make_shared<ControlNode>(rowNode.get(), Types::TypeId<size_t>());
			rowIndex->SetName("rowIndex");
			rowNode->AddChild(rowIndex);
			if (header->size() == 0)
			{
				for (const auto& column : rowNode->GetChildren())
				{
					if (column->TryGetAspect<FunctionArgumentAspect>())
						header->PushBack(Layout::AutoControl(column));
				}
			}
			else
			{
				std::map<size_t, std::shared_ptr<ControlNode>> indexedChildren;
				for (const std::shared_ptr<ControlNode>& child : rowNode->GetChildren())
				{
					if(const FunctionArgumentAspect* aspect = child->TryGetAspect<FunctionArgumentAspect>())
						indexedChildren.emplace(aspect->GetIndex(), child);
				}
				for (const auto& column : *header)
				{								
					std::shared_ptr<Layout::ControlElement> columnControlElement = std::dynamic_pointer_cast<Layout::ControlElement>(column);
					if (columnControlElement)
					{
						std::shared_ptr<ControlNode> child = rowNode->GetChild(columnControlElement->GetName());
						if (!child && !indexedChildren.empty())
						{
							auto ichild = indexedChildren.begin();
							auto child = ichild->second;
							Connect(child, columnControlElement);
							indexedChildren.erase(ichild);
						}
						else
						{
							if (columnControlElement->GetTypeInfo().IsValid())
							{
								auto item = std::make_shared<ControlNode>(rowNode.get(), columnControlElement->GetTypeInfo());
								Connect(item, columnControlElement);
								rowNode->AddChild(item);
							}
						}
					}
				}
			}
			node.AddAspect<RowAspect>(rowNode);
		}
		return priorState;
	}

	void ControlNodeHeaderAspectHandler::Connect(const std::shared_ptr<ControlNode>& node, const std::shared_ptr<Layout::ControlElement>& element)
	{
		if (element->GetTypeInfo().IsValid())
			node->InitializeType(element->GetTypeInfo(), false);
		node->SetName(element->GetName());
		node->SetCaption(element->GetCaption());
		if (element->GetAttribtes())
			node->SetCustomAttributeCollection(Attributes::AttributeCollection(*element->GetAttribtes()));
		element->SetControlNode(node);
	}

}}}}

