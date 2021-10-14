#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include <vector>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Description:
    Custom formatting of string
    */
    class MethodToStringAttribute
    {
    public:
        using ToStringFunction = std::function<std::string(const std::vector<std::pair<std::string, Objects::Object>>& args)>;
        MethodToStringAttribute(const ToStringFunction& function)
            : m_function(function)
        {}
        std::string ToString(const std::vector<std::pair<std::string, Objects::Object>>& args) const
        {
            return m_function(args);
        }
    private:
        ToStringFunction m_function;
    };

}}}}
