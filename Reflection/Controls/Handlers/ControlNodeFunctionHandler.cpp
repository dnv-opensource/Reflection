//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeFunctionHandler.h"
#include "Reflection/Controls/Aspects/FunctionArgumentAspect.h"
#include "Reflection/Controls/Aspects/FunctionAspect.h"
#include "Reflection/Controls/Aspects/IsFunctionalNodeAspect.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeFunctionHandler::OnInitializeMember(ControlNode& node, const Members::MemberPointer& member)
    {
        if (!node.GetManager() || !node.GetTypeLibrary() || !member || member->GetArity() < 1)
            return false;
        bool isStaticFunction = member->GetMemberType() == Members::MemberType::TypeStaticFunction;
        if (!isStaticFunction)
        {
            if (node.GetManager())
                node.GetManager()->OnInitializeType(node, member->GetArgumentInfo(0)->GetTypeInfo());
        }

        node.SetName(member->GetActualName());
        node.AddAspect<FunctionAspect>(member);
        if(!isStaticFunction)
            node.AddAspect<FunctionArgumentAspect>(member, 0);
        node.AddAspect<IsFunctionalNodeAspect>();

        size_t iStart = isStaticFunction ? 0 : 1;
        for (size_t i = iStart; i < member->GetArity(); ++i)
        {
            TryAddFunctionArgumentNode(node, member, i);
        }
        return true;
    }

}}}}

