#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Layout/ILayoutElement.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutHandler;
    class ILayoutControlContext {
    public:
        virtual ~ILayoutControlContext() {}
        virtual std::shared_ptr<ILayoutHandler> GetManager() const = 0;
        virtual bool HasLayout(const std::shared_ptr<Controls::ControlNode>& controlNode) const = 0;
        virtual void AddLayout(const std::shared_ptr<Controls::ControlNode>& controlNode) = 0;
        virtual std::shared_ptr<Controls::ControlNode> GetRootNode() const = 0;
    };
}}}}