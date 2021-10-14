//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodePropertyHandler.h"
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Controls/Aspects/PropertyAspect.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodePropertyHandler::OnInitializeMember(ControlNode& node, const Members::MemberPointer& member)
    {
        if (!member || (member->GetMemberType() & Members::MemberType::TypePropertyGetSet) == Members::MemberType::TypeNone)
            return false;
        
        node.SetName(member->GetName());
        if (node.GetManager())
            node.GetManager()->OnInitializeType(node, member->GetReturnType());
        node.AddAspect<PropertyAspect>(member);
        return true;
    }

}}}}

