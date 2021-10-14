//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeIncludeInViewAttributeHandler.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "ControlNodeHandlerUtilities.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    bool ControlNodeIncludeInViewAttributeHandler::OnInitialize(ControlNode& node, const Attributes::IncludeInViewAttribute& attribute, bool priorState)
    {
        if (priorState)
            return priorState;
        if (!node.GetTypeLibrary())
            return priorState;
        auto type = node.GetTypeLibrary()->LookupType(node.GetDecoratedTypeInfo().GetTypeInfo());
        if (!type)
            return priorState;
        for (const auto& name : attribute.GetMethods())
            TryAddMemberNode(node, type->Lookup(name, Variants::Variant()));
        return true;
    }
}}}}

