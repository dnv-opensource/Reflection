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
#include "../../Attributes/TupleAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	bool ControlNodeHeaderAspectHandler::OnInitialize(ControlNode& node, bool priorState)
	{
		if (const HeaderAspect* aspect = node.TryGetAspect<HeaderAspect>())
		{
			std::shared_ptr<Layout::HorizontalElements> header = aspect->GetHeader();
			auto containerAttribute = node.TryGetAttribute<Attributes::ContainerAttribute>();
			if (!containerAttribute)
				return priorState;
            if (node.TryGetAspect<RowAspect>())
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
						else if (child)
                        {
                            Connect(child, columnControlElement);
                            for (auto ichild = indexedChildren.begin(); ichild != indexedChildren.end(); ++ichild)
                            {
                                if (ichild->second == child)
                                {
                                    indexedChildren.erase(ichild);
                                    break;
                                }
                            }
                        }
				        else if (columnControlElement->GetTypeInfo().IsValid())
						{
							auto item = std::make_shared<ControlNode>(rowNode.get(), columnControlElement->GetTypeInfo());
							Connect(item, columnControlElement);
							rowNode->AddChild(item);
						}
					}
				}
			}
			node.AddAspect<RowAspect>(rowNode);
		}
		return priorState;
	}

    bool ControlNodeHeaderAspectHandler::CanSetName(ControlNode& node, const Layout::ControlElement& element) const
    {
        if (element.HasCaption())
            return true;
        if (node.GetName().empty())
            return true;
        if (!node.GetParent())
            return false;
        if (node.GetParent()->TryGetAttribute<Attributes::TupleAttribute>() != nullptr)
            return true;
        if (node.TryGetAspect<Controls::FunctionArgumentAspect>() == nullptr)
            return true;
        return false;
    }

    void ControlNodeHeaderAspectHandler::Connect(const std::shared_ptr<ControlNode>& node, const std::shared_ptr<Layout::ControlElement>& element)
	{
		if (element->GetTypeInfo().IsValid())
			node->InitializeType(element->GetTypeInfo(), false);
        node->SetCaption(element->GetCaption());
        if (CanSetName(*node, *element))
            node->SetName(element->GetName());
        else if (!element->GetName().empty())
            node->SetCaption(element->GetName());
		if (element->GetAttribtes())
			node->SetCustomAttributeCollection(Attributes::AttributeCollection(*element->GetAttribtes()));
		element->SetControlNode(node);
	}

}}}}

