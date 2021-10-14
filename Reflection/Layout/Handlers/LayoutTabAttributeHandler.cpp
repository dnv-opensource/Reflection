//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutTabAttributeHandler.h"
#include "Reflection/Attributes/TabAttribute.h"
#include "GroupCreators/TabGroupCreator.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElementGroupCreator> LayoutTabAttributeHandler::CreateGroupCreator(const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        if (auto tabAttribute = controlNode->TryGetAttribute<Attributes::TabAttribute>())
        {
            return std::make_shared<TabGroupCreator>(*tabAttribute);
        }
        return nullptr;
    }

}}}}

