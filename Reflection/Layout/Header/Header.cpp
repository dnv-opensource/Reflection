//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Header.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {

    Header::Header(const std::vector<std::shared_ptr<IColumn>>& columns, bool showHeader)
        : m_columns(columns)
        , m_showHeader(showHeader)
    {
    }

    Header::Header()
        : m_showHeader(false)
    {

    }

    const std::vector<std::shared_ptr<IColumn>>& Header::GetColumns() const
    {
        return m_columns;
    }

    bool Header::ShowHeader() const
    {
        return m_showHeader;
    }

}}}}


