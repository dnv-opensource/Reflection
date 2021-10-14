//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeSymmetryAttributeHandler.h"
#include "Reflection/Controls/Aspects/SymmetryAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeSymmetryAttributeHandler::OnInitialize(ControlNode& node, const Attributes::SymmetryAttribute& attribute, bool priorState)
    {
        auto child = std::make_shared<ControlNode>(&node, Types::TypeId<bool>());
        child->SetName(attribute.GetSymmetryName());
        child->SetCaption(attribute.GetSymmetryName());
        child->AddAspect<SymmetryAspect>(attribute.GetMasterProperty(), attribute.GetDocumentation());
        node.AddChild(child);
        return priorState;
    }

}}}}

