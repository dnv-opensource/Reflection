//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "VerticalElementAdder.h"
#include "Reflection/Layout/Visitors/HasMemberVisitor.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    void VerticalElementAdder::AddElement(const LayoutElementPointer& element, const Attributes::AttributeCollection& attributeCollection)
    {
        if (!m_layoutElement)
            m_layoutElement = std::make_shared<VerticalElements>(element);
        else
            m_layoutElement->PushBack(element);
    }

    bool VerticalElementAdder::HasMember(const Members::MemberPointer& member) const
    {
        if (!m_layoutElement)
            return false;
        HasMemberVisitor visitor(member);
        m_layoutElement->Accept(visitor);
        return visitor.GetResult();
    }

    LayoutElementPointer VerticalElementAdder::GetLayoutElement() const
    {
        return m_layoutElement;
    }

}}}}

