#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Layout/ILayoutElement.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutElementGroupCreator {
    public:
        virtual ~ILayoutElementGroupCreator() {}
        virtual void AddLayoutElement(const std::shared_ptr<ILayoutElement>& layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode) = 0;
        virtual std::shared_ptr<ILayoutElement> GetGroup() const = 0;
    };
}}}}