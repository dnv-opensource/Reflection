//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ForwardingLayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    ForwardingLayoutControlContext::ForwardingLayoutControlContext(ILayoutControlContext& context)
        : m_context(context)
    {

    }

    std::shared_ptr<ILayoutHandler> ForwardingLayoutControlContext::GetManager() const
    {
        return m_context.GetManager();
    }

    bool ForwardingLayoutControlContext::HasLayout(const std::shared_ptr<Controls::ControlNode>& controlNode) const
    {
        return m_context.HasLayout(controlNode);
    }

    void ForwardingLayoutControlContext::AddLayout(const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        m_context.AddLayout(controlNode);
    }

    std::shared_ptr<Controls::ControlNode> ForwardingLayoutControlContext::GetRootNode() const
    {
        return m_context.GetRootNode();
    }

}}}}

