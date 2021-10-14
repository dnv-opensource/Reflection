#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionOfElements.h"
#include "ElementWithOneChild.h"
#include "LayoutContext.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Controls\ControlNode.h"
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    //An overload selector needs to at least contain as children either:
    //1. a Selector and a DynamicArea
    //2. a TabControl.
    class OverloadSelector : public TypedLayoutElement<OverloadSelector, ElementWithOneChild>
    {
    public:
        //Needed in unit tests
        OverloadSelector() {}
        OverloadSelector(OverloadSelector&&) = default;
        OverloadSelector(const OverloadSelector& other) = default;
        OverloadSelector& operator=(OverloadSelector&&) = default;
        OverloadSelector& operator=(const OverloadSelector& other) = default;

        template<typename T>
        OverloadSelector(const std::string& name, const T& element) 
            : TypedLayoutElement<OverloadSelector, ElementWithOneChild>(element) 
            , m_name(name)
        {}
        template<typename T>
        OverloadSelector(const T& element) 
            : TypedLayoutElement<OverloadSelector, ElementWithOneChild>(element) 
        {}
        
        OverloadSelector& SetMembers(const std::list<Members::MemberPointer>& members)
        {
            m_members = members;
            return *this;
        }
        OverloadSelector& SetMembers(const std::set<Members::MemberPointer>& members)
        {
            m_members = std::list<Members::MemberPointer>(members.begin(), members.end());
            return *this;
        }
        std::set<Members::MemberPointer> GetMembers() const
        {
            return std::set<Members::MemberPointer>(m_members.begin(), m_members.end());
        }
        const std::list<Members::MemberPointer>& GetOrderedMembers() const {return m_members;}
        LayoutContext& GetContext() { return m_context; }
        const LayoutContext& GetContext() const { return m_context; }
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetCaption() const { return m_caption; }
        void SetCaption(const std::string& caption) { m_caption = caption; }
        const std::shared_ptr<Controls::ControlNode>& GetControlNode() const { return m_controlNode; }
        void SetControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode)
        {
            m_controlNode = controlNode;
            if (m_controlNode)
            {
                m_name = m_controlNode->GetName();
                m_caption = m_controlNode->GetCaption();
            }
        }
        std::shared_ptr<Controls::ControlNode> TryGetMatchingControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode)
        {
            if (!controlNode)
                return nullptr;
            if (controlNode == m_controlNode)
                return controlNode;
            if (m_name.empty())
                return controlNode;
            if (_strcmpi(controlNode->GetName().c_str(), m_name.c_str()) == 0)
                return controlNode;
            if (m_caption == "__USENAME__" && m_name == controlNode->GetCaption())
                return controlNode;
            return controlNode->LookupRelatedNode(m_name);
        }
    private:
        std::list<Members::MemberPointer> m_members;
        std::shared_ptr<Controls::ControlNode> m_controlNode;
        LayoutContext m_context;
        std::string m_name;
        std::string m_caption;
    };
}}}}