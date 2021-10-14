#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Layout/Header/Header.h"
#include "Reflection/Layout/Header/DefaultColumn.h"
#include "Reflection/Layout/Header/CallbackColumn.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class HeaderAttribute : public IAttribute {
    public:
        template<typename... Columns>
        HeaderAttribute(const Columns&... columns)
            : m_header(columns...)
        {}
        const Layout::Header& GetHeader() const { return m_header; }
    private:
        Layout::Header m_header;
    };
}}}}
