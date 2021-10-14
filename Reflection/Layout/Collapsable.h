#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedLayoutElement.h"
#include "ElementWithOneChild.h"
#include "Reflection/Controls/ControlNode.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Collapsable : public TypedLayoutElement<Collapsable, ElementWithOneChild>
    {
    public:
        Collapsable(const Collapsable& other) = default;
        Collapsable(Collapsable&& other) = default;
        Collapsable& operator=(const Collapsable& other) = default;
        Collapsable& operator=(Collapsable&& other) = default;
        //Needed in unit tests
        Collapsable() {}
        template<typename T>
        Collapsable(const T& element) : TypedLayoutElement<Collapsable, ElementWithOneChild>(element) {}
        void SetControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode) { m_controlNode = controlNode; }
        std::shared_ptr<Controls::ControlNode> GetControlNode() const { return m_controlNode; }
    private:
        std::shared_ptr<Controls::ControlNode> m_controlNode;
    };
}}}}
