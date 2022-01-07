#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutElement.h"
#include "TypedLayoutElement.h"
#include "ElementWithOneChild.h"
#include "Reflection/Controls/ControlNode.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class EmbeddedElement : public TypedLayoutElement<EmbeddedElement, ElementWithOneChild>
    {
    public:
        EmbeddedElement(EmbeddedElement&&) = default;
        EmbeddedElement(const EmbeddedElement&) = default;
        EmbeddedElement& operator=(EmbeddedElement&&) = default;
        EmbeddedElement& operator=(const EmbeddedElement&) = default;

        //This constructor is used when you want to embed all methods of a nested member.
        EmbeddedElement() : m_isProxy(false) {}
        template<typename T>
        explicit EmbeddedElement(const std::string& name, const T& child)
            : m_name(name)
            , m_isProxy(false)
            , TypedLayoutElement<EmbeddedElement, ElementWithOneChild>(child)
        {}
        template<typename T>
        explicit EmbeddedElement(const T& child)
            : m_isProxy(false)
            , TypedLayoutElement<EmbeddedElement, ElementWithOneChild>(child)
        {}
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }
        bool IsProxy() const { return m_isProxy; }
        void SetProxy(bool proxy) { m_isProxy = proxy; }
        std::shared_ptr<Controls::ControlNode> TryGetMatchingControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode)
        {
            if (!controlNode)
                return nullptr;
            if (controlNode == m_controlNode)
                return controlNode;
            if (m_name.empty())
                return controlNode;
            if (_strcmpi(controlNode->GetName().c_str(),m_name.c_str()) == 0)
                return controlNode;
            return controlNode->LookupRelatedNode(m_name);
        }
        const std::shared_ptr<Controls::ControlNode>& GetControlNode() const { return m_controlNode; }
        void SetControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode)
        {
            m_controlNode = controlNode;
            if (m_controlNode && !m_controlNode->GetCaption().empty())
            {
                m_name = m_controlNode->GetCaption();
            }
        }
    private:
        std::shared_ptr<Controls::ControlNode> m_controlNode;
        bool m_isProxy;
        std::string m_name;
    };

}}}}
