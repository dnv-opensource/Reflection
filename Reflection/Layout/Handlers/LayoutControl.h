#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Controls/ControlNode.h"
#include "Reflection/Layout/ILayoutElement.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutManager;
    class REFLECTION_IMPORT_EXPORT LayoutControl {
    public:
        LayoutControl(const std::shared_ptr<Controls::ControlNode>& root);
        std::shared_ptr<ILayoutElement> GetLayout() const;
    private:
        std::shared_ptr<ILayoutElement> m_layout;
        std::shared_ptr<Controls::ControlNode> m_root;
        std::shared_ptr<LayoutManager> m_layoutManager;
    };
}}}}