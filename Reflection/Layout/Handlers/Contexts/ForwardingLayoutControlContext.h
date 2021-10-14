#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutControlContext.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutHandler;
    class ForwardingLayoutControlContext : public ILayoutControlContext
    {
    public:
        ForwardingLayoutControlContext(ILayoutControlContext& context);
        std::shared_ptr<ILayoutHandler> GetManager() const override;
        bool HasLayout(const std::shared_ptr<Controls::ControlNode>& controlNode) const override;
        void AddLayout(const std::shared_ptr<Controls::ControlNode>& controlNode) override;
        std::shared_ptr<Controls::ControlNode> GetRootNode() const override;
    private:
        ILayoutControlContext& m_context;
    };
}}}}