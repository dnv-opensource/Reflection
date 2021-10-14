#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutElementGroupCreator.h"
#include "Reflection/Layout/CollectionOfElements.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class CollectionOfElementsGroupCreator : public ILayoutElementGroupCreator {
    public:
        CollectionOfElementsGroupCreator(const std::shared_ptr<CollectionOfElements>& group);
        virtual void AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode);
        virtual std::shared_ptr<ILayoutElement> GetGroup() const;
    private:
        std::shared_ptr<CollectionOfElements> m_group;
    };
}}}}