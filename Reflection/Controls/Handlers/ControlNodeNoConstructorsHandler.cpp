//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeNoConstructorsHandler.h"
#include "ControlNodeHandlerUtilities.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"
#include "Reflection/Controls/Aspects/FunctionSelectorAspect.h"
#include "Reflection/Controls/Aspects/PropertyAspect.h"
#include "Reflection/Controls/Aspects/IndexedPropertyAspect.h"
#include "Reflection/Controls/Aspects/FunctionAspect.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "Reflection/TypeLibraries/TypeWithBases.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNoConstructorsHandler::OnInitialize(ControlNode& node, bool priorState)
    {
        if(!priorState && ShouldAddMembers(node))
        {
            auto type = node.GetTypeLibrary()->LookupType(node.GetDecoratedTypeInfo().GetTypeInfo());
            if (!type)
                return priorState;
            TypeLibraries::TypeWithBases wrapperType(type, node.GetTypeLibrary());
            for (const auto& memberName : wrapperType.GetUnsortedMembers())
                TryAddMemberNode(node, wrapperType.Lookup(memberName, Variants::Variant()));
            return true;
        }
        return priorState;
    }

    bool ControlNoConstructorsHandler::ShouldAddMembers(ControlNode& node) const
    {
        if (!node.GetTypeLibrary())
            return false;
        if (node.TryGetAspect<ConstructorSelectorAspect>())
            return false;
        if (node.TryGetAspect<FunctionSelectorAspect>())
            return false;
        if (node.TryGetAspect<PropertyAspect>())
            return false;
        if (node.TryGetAspect<IndexedPropertyAspect>())
            return false;
        if (node.GetParent())
            return false;
        if (node.TryGetAspect<FunctionAspect>())
            return true;
        if (!node.GetChildren().empty())
            return false;
        if (!Utilities::ConstructorsFromTypeService::GetService(node.GetTypeLibrary())->CollectAllConstructorMembersOfThisAndDerivedClasses(node.GetDecoratedTypeInfo(), true).empty())
            return false;
        return true;
    }

}}}}
