//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutVerticalElementsGroupCreatorHandler.h"
#include "Reflection/Layout/VerticalElements.h"
#include "GroupCreators/CollectionOfElementsGroupCreator.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElementGroupCreator> LayoutVerticalElementsGroupCreatorHandler::CreateGroupCreator(const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        return std::make_shared<CollectionOfElementsGroupCreator>(std::make_shared<VerticalElements>());
    }

}}}}

