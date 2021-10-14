#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "IAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class ActionAttribute : public IAttribute
    {
    public:
        ActionAttribute(const std::string& name) : m_name(name) {}
        const std::string& GetName() const { return m_name; }
    private:
        std::string m_name;
    };
}}}}