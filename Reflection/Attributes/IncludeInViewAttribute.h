#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <list>
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    */
    class IncludeInViewAttribute 
    {
    private:
        IncludeInViewAttribute() {}
    public:
        template<typename... Args>
        IncludeInViewAttribute(const std::string& arg, Args&&... args)
            : IncludeInViewAttribute(std::forward<Args>(args)...)
        {
            m_methodsToInclude.emplace_front(arg);
        }
        const std::list<std::string>& GetMethods() const { return m_methodsToInclude; }
    private:
        std::list<std::string> m_methodsToInclude;
    };
}}}}
