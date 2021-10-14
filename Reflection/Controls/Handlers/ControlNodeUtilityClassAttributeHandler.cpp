//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeUtilityClassAttributeHandler.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"
#include "Reflection/Controls/Aspects/FunctionSelectorAspect.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/TypeLibraries/TypeWithBases.h"
#include "ControlNodeHandlerUtilities.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeUtilityClassAttributeHandler::OnInitialize(ControlNode& node, const Attributes::UtilityClassAttribute& attribute, bool priorState)
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
        node.SetName("__this__");
        node.SetCaption("UtilityClass");
        TypeLibraries::TypeWithBases wrapperType(type, node.GetTypeLibrary());
        for (const auto& memberName : wrapperType.GetUnsortedMembers())
            TryAddMemberNode(node, wrapperType.Lookup(memberName, Variants::Variant()));
        auto constructor = type->Lookup(Members::ConstructorString);
        if (constructor && constructor->GetOverloads().size() == 1)
            node.AddAspect<ConstructorAspect>(constructor);
        return true;
    }

}}}}

