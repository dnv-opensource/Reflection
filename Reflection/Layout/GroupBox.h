#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionOfElements.h"
#include "TypedLayoutElement.h"
#include "CheckBox.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {    
    class GroupBox : public TypedLayoutElement<GroupBox, CollectionOfElements>
    {
    public:
        GroupBox(const GroupBox& other) = default;
        GroupBox(GroupBox&& other) = default;
        GroupBox& operator=(const GroupBox& other) = default;
        GroupBox& operator=(GroupBox&& other) = default;

        template<typename... ElementsT>
        explicit GroupBox(const std::string& groupName, const ElementsT&... elements) 
            : TypedLayoutElement<GroupBox, CollectionOfElements>(elements...)
        {
            SetName(groupName);
        }
        template<typename... ElementsT>
        explicit GroupBox(const CheckBox& enabler, const ElementsT&... elements)
            : GroupBox(std::make_shared<CheckBox>(enabler), elements...)
        {}
        template<typename... ElementsT>
        explicit GroupBox(const std::pair<std::string, CheckBox>& enablerAndHeader, const ElementsT&... elements)
            : GroupBox(enablerAndHeader.second, elements...)
        {
            SetName(enablerAndHeader.first);
        }
        template<typename... ElementsT>
        explicit GroupBox(const std::shared_ptr<CheckBox>& enabler, const ElementsT&... elements)
            : GroupBox("", elements...)
        {
            m_enabler = enabler;
        }
        GroupBox AddDocumentation(const std::string& doc)
        {
            m_documentation = doc;
            return *this;
        }
        std::string GetGroupName() const { 
            if (GetName().empty() && m_enabler)
                return m_enabler->GetCaption();
            return GetName(); 
        }
        std::string GetDocumentation() const { return m_documentation; }
        void SetEnabler(const std::shared_ptr<CheckBox>& enabler) { m_enabler = enabler; }
        std::shared_ptr<CheckBox> GetEnabler() const { return m_enabler; }
        bool Merge(const GroupBox& other)
        {
            if (m_enabler && other.m_enabler)
            {
                if(m_enabler->GetName() != other.m_enabler->GetName())
                    return false;
            }
            else if (other.m_enabler)
                m_enabler = other.m_enabler;
            if (m_documentation.empty())
                m_documentation = other.m_documentation;
            else if(!other.m_documentation.empty() && m_documentation != other.m_documentation)
                m_documentation += "\n" + other.m_documentation;
            for (auto content : other)
            {
                PushBack(content);
            }
            return true;
        }
    private:
        ClonableLayoutPointer<CheckBox> m_enabler;
        std::string m_documentation;
    };
}}}}
