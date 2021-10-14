#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include <set>
#include <algorithm>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class ContextAttribute {
    public:
        template<typename... Args>
        ContextAttribute(const std::string& context, const Args&... args) 
            : ContextAttribute(args...)
        {
            m_contexts.insert(context);
        }
        bool SharedContext(const ContextAttribute& other) const
        {
            std::set<std::string> commonContexts;
            std::set_intersection(m_contexts.begin(), m_contexts.end(), other.m_contexts.begin(), other.m_contexts.end(), std::inserter(commonContexts, commonContexts.begin()));
            return !commonContexts.empty();
        }
    private:
        ContextAttribute() {}
        std::set<std::string> m_contexts;
    };
}}}}
