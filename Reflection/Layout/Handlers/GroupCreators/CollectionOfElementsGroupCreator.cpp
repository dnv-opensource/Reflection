//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionOfElementsGroupCreator.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    CollectionOfElementsGroupCreator::CollectionOfElementsGroupCreator(const std::shared_ptr<CollectionOfElements>& group)
        : m_group(group)
    {

    }

    void CollectionOfElementsGroupCreator::AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        if (m_group)
            m_group->PushBack(layoutElement);
    }

    std::shared_ptr<ILayoutElement> CollectionOfElementsGroupCreator::GetGroup() const
    {
        return m_group;
    }

}}}}

