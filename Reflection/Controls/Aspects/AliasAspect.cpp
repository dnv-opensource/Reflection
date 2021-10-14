//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "AliasAspect.h"
#include "Reflection/Controls/ControlNode.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    AliasAspect::AliasAspect(const std::shared_ptr<ControlNode>& aliasNode)
        : m_aliasNode(aliasNode)
    {

    }

    const Attributes::AttributeCollection& AliasAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    bool AliasAspect::operator==(const AliasAspect& other) const
    {
        return m_aliasNode == other.m_aliasNode;
    }

    std::string AliasAspect::ToString() const
    {
        std::string result = "AliasAspect(";
        if (m_aliasNode)
            result += m_aliasNode->GetName();
        else
            result += "nullptr";
        result += ")";
        return result;
    }

    const std::shared_ptr<ControlNode>& AliasAspect::GetAliasNode() const
    {
        return m_aliasNode;
    }

}}}}

