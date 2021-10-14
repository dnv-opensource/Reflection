//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLambdaPropertyAspectHandler.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/Aspects/LambdaPropertyAspect.h"
#include "Reflection/Controls/Aspects/TypeAspect.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLambdaPropertyAspectHandler::OnInitialize(ControlNode& node, bool priorState)
    {
        if (const auto* aspect = node.TryGetAspect<LambdaPropertyAspect>())
        {
            node.GetManager()->OnInitializeType(node, aspect->GetDecoratedTypeInfo());
            node.SetName(aspect->GetName());
            if (node.GetName().empty())
            {
                if (auto typeAspect = node.TryGetAspect<TypeAspect>())
                {
                    if (Reflection::TypeLibraries::TypePointer type = typeAspect->GetType())
                    {
                        for (const auto& name : type->GetUnsortedMembers())
                            TryAddMemberNode(node, type->Lookup(name, Variants::Variant()));
                    }
                }
                return true;                
            }
        }
        return priorState;
    }

}}}}

