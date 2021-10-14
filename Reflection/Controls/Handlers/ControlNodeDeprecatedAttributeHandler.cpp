//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeDeprecatedAttributeHandler.h"
#include "Reflection/Attributes/DeprecatedAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeDeprecatedAttributeHandler::OnInitializeMember(ControlNode& node, const Members::MemberPointer& member)
    {
        if (!member)
            return false;
        //Terminate initialization of member if the member is deprecated.
        return member->GetAttributeCollection().HasAttribute<Attributes::DeprecatedAttribute>();
    }

}}}}

