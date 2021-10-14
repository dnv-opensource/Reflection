//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutControlContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    LayoutControlContext::LayoutControlContext(const std::shared_ptr<ILayoutHandler>& manager, const std::shared_ptr<Controls::ControlNode>& rootNode)
        : m_manager(manager)
        , m_rootNode(rootNode)
    {

    }

    std::shared_ptr<ILayoutHandler> LayoutControlContext::GetManager() const
    {
        return m_manager;
    }

    bool LayoutControlContext::HasLayout(const std::shared_ptr<Controls::ControlNode>& controlNode) const
    {
        return m_hasLayout.count(controlNode)>0;
    }

    void LayoutControlContext::AddLayout(const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        m_hasLayout.insert(controlNode);
    }

    std::shared_ptr<Controls::ControlNode> LayoutControlContext::GetRootNode() const
    {
        return m_rootNode;
    }

}}}}

