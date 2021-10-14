//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "HasMemberVisitor.h"
#include "Reflection/Layout/ControlElement.h"
#include "Reflection/Layout/CollectionOfElements.h"
#include "Reflection/Layout/GroupBox.h"
#include "Reflection/Layout/ElementWithOneChild.h"
#include "Reflection/Layout/EmbeddedElement.h"
#include "Reflection/Layout/Table.h"
#include "Reflection/Layout/Symmetry.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    HasMemberVisitor::HasMemberVisitor(const Members::MemberPointer& member)
        : m_result(false)
    {
        if (member)
            for (const auto& member : member->GetOverloads())
                m_members.insert(member);
    }

    HasMemberVisitor::HasMemberVisitor(const std::set<Members::MemberPointer>& members)
        : m_result(false)
        , m_members(members)
    {

    }

    void HasMemberVisitor::Visit(const ControlElement& lhs)
    {
        if (m_members.count(lhs.GetContext().getter) > 0)
            m_result = true;
        if (m_members.count(lhs.GetContext().setter) > 0)
            m_result = true;
        if (m_members.count(lhs.GetContext().function) > 0)
            m_result = true;
    }

    void HasMemberVisitor::Visit(const CollectionOfElements& lhs)
    {
        for (const auto& child : lhs)
            child->Accept(*this);
    }

    void HasMemberVisitor::Visit(const GroupBox& lhs)
    {
        for (const auto& child : lhs)
            child->Accept(*this);
    }

    void HasMemberVisitor::Visit(const ElementWithOneChild& lhs)
    {
        lhs.GetChild()->Accept(*this);
    }

    void HasMemberVisitor::Visit(const EmbeddedElement& lhs)
    {
        lhs.GetChild()->Accept(*this);
    }

    void HasMemberVisitor::Visit(const Table& lhs)
    {
        if (m_members.count(lhs.GetContext().getter) > 0)
            m_result = true;
        if (m_members.count(lhs.GetContext().setter) > 0)
            m_result = true;
        if (m_members.count(lhs.GetContext().function) > 0)
            m_result = true;
    }

    void HasMemberVisitor::Visit(const Symmetry& lhs)
    {
        lhs.GetChild()->Accept(*this);
    }

    bool HasMemberVisitor::GetResult() const
    {
        return m_result;
    }

}}}}

