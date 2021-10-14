#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "ILayoutHandler.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class LayoutManager : public ILayoutHandler
    {
    public:
        LayoutManager(bool addDefaultHandlers = true);
        std::shared_ptr<ILayoutElement> ModifyLayout(std::shared_ptr<ILayoutElement> element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context) override;
        std::shared_ptr<ILayoutElementGroupCreator> CreateGroupCreator(const std::shared_ptr<Controls::ControlNode>& controlNode) override;
        void AddHandler(ILayoutHandler* handler);
    private:
        std::vector<std::unique_ptr<ILayoutHandler>> m_handlers;
    };
}}}}