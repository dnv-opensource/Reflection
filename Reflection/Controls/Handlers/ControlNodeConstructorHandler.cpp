//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeConstructorHandler.h"
#include "Reflection/Utilities/ConstructorsFromTypeExtracter.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/PropertyAspect.h"
#include "Reflection/Controls/Aspects/FunctionArgumentAspect.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeConstructorHandler::OnInitializeMember(ControlNode& node, const Members::MemberPointer& member)
    {
        if (!node.GetManager() || !node.GetTypeLibrary())
            return false;
        if (!member || !Utilities::ConstructorsFromTypeExtracter(node.GetTypeLibrary()).IsConstructorMember(member, true))
            return false;
        //At this point, member is guaranteed to be a constructor.
        if (node.GetManager())
            node.GetManager()->OnInitializeType(node, member->GetReturnType());
        if(!node.TryGetAspect<PropertyAspect>())
            node.SetName(member->GetActualName());
        node.AddAspect<ConstructorAspect>(member);
        
        for (size_t i = 0; i < member->GetArity(); ++i)
            TryAddFunctionArgumentNode(node, member, i);
        return true;
    }

}}}}

