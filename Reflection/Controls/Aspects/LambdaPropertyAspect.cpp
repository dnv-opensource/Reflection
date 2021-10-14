//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LambdaPropertyAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool LambdaPropertyAspect::operator==(const LambdaPropertyAspect& other) const
    {
        return m_name == other.m_name;
    }

    const Attributes::AttributeCollection& LambdaPropertyAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string LambdaPropertyAspect::ToString() const
    {
        return "LambdaPropertyAspect(\"" + m_name+ "\")";
    }

    Contents::Content LambdaPropertyAspect::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify /*= true*/)
    {
        if (trySimplify)
            return m_callback(parent);
        else if (SupportAssign())
        {
            std::vector<Contents::Content> children;
            for (Contents::Content content : parent.Split())
                children.push_back(GetNestedProperty(content));
            return Contents::Content(children);
        }
        else
            return parent.CopyChangeType(m_decoratedTypeInfo);
    }

    Contents::Content LambdaPropertyAspect::TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value)
    {
        return GetNestedProperty(parent).Assign(value);
    }

    bool LambdaPropertyAspect::SupportAssign() const
    {
        return !m_name.empty();
    }

    const Types::DecoratedTypeInfo& LambdaPropertyAspect::GetDecoratedTypeInfo() const
    {
        return m_decoratedTypeInfo;
    }

    std::string LambdaPropertyAspect::GetName() const
    {
        return m_name;
    }

    Contents::Content LambdaPropertyAspect::GetNestedProperty(const Contents::Content& parent) const
    {
        if (!m_name.empty())
            return Contents::Content(parent.GetScriptText() + "." + m_name, m_decoratedTypeInfo);
        else
            return parent;
    }

}}}}

