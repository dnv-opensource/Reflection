//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutControl.h"
#include "LayoutManager.h"
#include "Contexts/LayoutControlContext.h"
#include "Reflection/Layout/AutoControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    LayoutControl::LayoutControl(const std::shared_ptr<Controls::ControlNode>& root)
        : m_root(root)
        , m_layoutManager(std::make_shared<LayoutManager>())
    {
        LayoutControlContext context(m_layoutManager, root);
        m_layout = m_layoutManager->ModifyLayout(std::make_shared<AutoControl>(m_root->GetName(), m_root->GetCaption()), m_root, context);
    }

    std::shared_ptr<DNVS::MoFa::Reflection::Layout::ILayoutElement> LayoutControl::GetLayout() const
    {
        return m_layout;
    }

}}}}

