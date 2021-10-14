#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities/Visitors/UnaryVisitorWithReturn.h"
#include "Reflection/Config.h"
#include "Reflection/Members/IMember.h"
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ControlElement;
    class CollectionOfElements;
    class GroupBox;
    class ILayoutElement;
    class EmbeddedElement;
    class ElementWithOneChild;
    class Table;
    class Symmetry;
    class ILayoutElement;
#pragma warning(push)
#pragma warning(disable:4275)

    class REFLECTION_IMPORT_EXPORT HasMemberVisitor : public TypeUtilities::Visitors<
        ControlElement, 
        CollectionOfElements, 
        GroupBox, 
        ElementWithOneChild,
        EmbeddedElement,
        Table,
        Symmetry
        >
    {
    public:
        HasMemberVisitor(const Members::MemberPointer& member);
        HasMemberVisitor(const std::set<Members::MemberPointer>& members);
        virtual void Visit(const ControlElement& lhs) override;
        virtual void Visit(const CollectionOfElements& lhs) override;
        virtual void Visit(const GroupBox& lhs) override;
        virtual void Visit(const ElementWithOneChild& lhs) override;
        virtual void Visit(const EmbeddedElement& lhs) override;
        virtual void Visit(const Table& lhs) override;
        virtual void Visit(const Symmetry& lhs) override;
        bool GetResult() const;
    private:
        std::set<Members::MemberPointer> m_members;
        bool m_result;
    };
#pragma warning(pop)
}}}}