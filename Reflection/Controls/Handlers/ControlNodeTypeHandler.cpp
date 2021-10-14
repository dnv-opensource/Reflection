//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeTypeHandler.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Controls/Aspects/TypeAspect.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "../../Attributes/LayoutAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    void ControlNodeTypeHandler::OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& decoratedTypeInfo)
    {
        node.SetDecoratedTypeInfo(decoratedTypeInfo);
        if (node.GetTypeLibrary())
        {
            auto type = node.GetTypeLibrary()->LookupType(decoratedTypeInfo.GetTypeInfo());
			if (type)
			{
				node.AddAspect<TypeAspect>(type);
				if (node.GetName().empty())
					node.SetName(type->GetName());
			}
			else
				node.RemoveAspect<TypeAspect>();
        }
    }

    bool ControlNodeTypeHandler::OnInitialize(ControlNode& node, bool priorState)
    {
        if (priorState)
            return priorState;
        if (!node.GetManager())
            return priorState;
        if (node.GetParent())
            return priorState;
        if (node.GetChild("__SELECTOR__"))
            return priorState;
        if (node.TryGetAttribute<Attributes::LayoutAttribute>())
            return priorState;
        if (!node.GetChildren().empty())
            return priorState;
        if (node.HasMember())
            return priorState;
        auto service = Utilities::ConstructorsFromTypeService::GetService(node.GetTypeLibrary());
        if (service)
        {
            auto constructors = service->CollectAllConstructorMembersOfThisAndDerivedClasses(node.GetDecoratedTypeInfo(), true);
            if (constructors.size() == 1)
                node.GetManager()->OnInitializeMember(node, *constructors.begin());
        }
        return priorState;
    }

}}}}

