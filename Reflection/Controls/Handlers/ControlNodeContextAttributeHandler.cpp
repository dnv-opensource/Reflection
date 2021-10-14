//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeContextAttributeHandler.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"
#include "Reflection/Controls/Aspects/FunctionSelectorAspect.h"
#include "Reflection/TypeLibraries/TypeWithBases.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeContextAttributeHandler::OnInitialize(ControlNode& node, const Attributes::ContextAttribute& attribute, bool priorState)
    {
        if (priorState)
            return priorState;
        if (!node.GetTypeLibrary())
            return priorState;
        if (node.TryGetAspect<ConstructorSelectorAspect>())
            return priorState;
        if (node.TryGetAspect<FunctionSelectorAspect>())
            return priorState;
        auto type = node.GetTypeLibrary()->LookupType(node.GetDecoratedTypeInfo().GetTypeInfo());
        if (!type)
            return priorState;

        TypeLibraries::TypeWithBases wrapperType(type, node.GetTypeLibrary());
        for (const auto& memberName : wrapperType.GetUnsortedMembers())
        {
            if (node.GetChild(memberName))
                continue;
            auto member = wrapperType.Lookup(memberName, Variants::Variant());
            if (member && 
                member->GetAttributeCollection().HasAttribute<Attributes::ContextAttribute>() && 
                attribute.SharedContext(member->GetAttributeCollection().GetAttribute<Attributes::ContextAttribute>()))
            {
                TryAddMemberNode(node, member);
            }
        }
        return true;
    }

}}}}

