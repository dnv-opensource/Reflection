#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultOptions.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class DefaultMemberAttribute : public IAttribute
    {
    public:
        DefaultMemberAttribute(const Impl::StoreDefault& storeDefault)
            : m_storeDefault(true)
        {}
        DefaultMemberAttribute()
            : m_storeDefault(false)
        {}
        bool StoreDefault() const { return m_storeDefault; }
    private:
        bool m_storeDefault;
    };

}}}}

