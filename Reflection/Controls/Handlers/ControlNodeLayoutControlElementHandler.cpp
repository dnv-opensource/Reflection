//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutControlElementHandler.h"
#include "ControlNodeHandlerUtilities.h"
#include "Reflection/Controls/Aspects/CustomAttributeCollectionAspect.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "Reflection/Controls/Aspects/HeaderAspect.h"
#include "../Aspects/CanAddAndRemoveRowsAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutControlElementHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element, bool priorState)
    {
        if (auto relatedNode = LookupRelatedNode(node, element))
        {
            SetData(*relatedNode, *element);
            return priorState;
        }
        auto memberPair = node.LookupRelatedMember(element->GetName());
        if (memberPair.first && memberPair.second)
        {
            auto child = std::make_shared<ControlNode>(memberPair.first, memberPair.second);
			SetData(*child, *element);
			memberPair.first->AddChild(child);
            return priorState;
        }
		if (element->GetTypeInfo().IsValid())
		{
			auto child = std::make_shared<ControlNode>(&node, element->GetTypeInfo());
			SetData(*child, *element);
			node.AddChild(child);
			return priorState;
		}
        if (element->GetCaption() == "__USENAME__")
            SetData(node, *element);
        return priorState;
    }

    bool ControlNodeLayoutControlElementHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::Grid>& element, bool priorState)
	{
		if (auto relatedNode = LookupRelatedNode(node, element))
		{
			SetGridData(*relatedNode, *element);
			return priorState;
		}
		auto memberPair = node.LookupRelatedMember(element->GetName());
		if (memberPair.first && memberPair.second)
		{
			auto child = std::make_shared<ControlNode>(memberPair.first, memberPair.second);
			SetGridData(*child, *element);
			memberPair.first->AddChild(child);
			return priorState;
		}
		if (element->GetTypeInfo().IsValid())
		{
			auto child = std::make_shared<ControlNode>(&node, element->GetTypeInfo());
			SetGridData(*child, *element);
			node.AddChild(child);
			return priorState;
		}
		if (Attributes::GetPointerToAttributeOrNull<Attributes::ContainerAttribute>(node.GetTypeLibrary(), node.GetDecoratedTypeInfo()))
		{
			if (element->GetCaption() == "__USENAME__")
				SetGridData(node, *element);
			return priorState;
		}
		auto child = std::make_shared<ControlNode>(&node, Types::TypeId<void>());
		SetGridData(*child, *element);
		node.AddChild(child);
		return priorState;
	}

	void ControlNodeLayoutControlElementHandler::SetData(ControlNode& node, const Layout::ControlElement& element)
    {
        if (element.GetCaption() == "__USENAME__")
        {
            if (node.GetName() == node.GetCaption())
                node.SetCaption(element.GetName());
        }
        else
            node.SetCaption(element.GetCaption());
		if (node.GetName().empty())
			node.SetName(element.GetName());
		if (element.GetTypeInfo().IsValid())
			node.InitializeType(element.GetTypeInfo(), false);
		if (element.GetAttribtes())
			node.AddAspect<CustomAttributeCollectionAspect>(Attributes::AttributeCollection(*element.GetAttribtes()));
    }


	void ControlNodeLayoutControlElementHandler::SetGridData(ControlNode& node, const Layout::Grid& element)
	{
		SetData(node, element);
		if (!element.GetFixedGrid())
		{
			node.AddAspect<HeaderAspect>(element.GetHeader());
		}
		if (element.GetCanAddRemoveRows())
			node.AddAspect<CanAddAndRemoveRowsAspect>();
	}

}}}}

