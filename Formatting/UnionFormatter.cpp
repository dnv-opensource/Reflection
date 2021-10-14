//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnionFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    UnionFormatter::UnionFormatter(const IStringFormatter& lhs, const IStringFormatter& rhs)
        : m_lhs(lhs.Clone())
        , m_rhs(rhs.Clone())
    {

    }

    UnionFormatter::UnionFormatter(const UnionFormatter& other)
        : m_lhs(other.m_lhs->Clone())
        , m_rhs(other.m_rhs->Clone())
    {

    }

    UnionFormatter::~UnionFormatter()
    {

    }

    std::string UnionFormatter::Format(const std::string& text) const
    {
        return m_rhs->Format(m_lhs->Format(text));
    }

    IStringFormatter* UnionFormatter::Clone() const
    {
        return new UnionFormatter(*m_lhs, *m_rhs);
    }

}}}

