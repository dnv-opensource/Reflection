#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <functional>
#include <list>
#include "Reflection/Objects/Object.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class ExampleAttribute
    {
    public:
        template<typename... ExamplesT>
        ExampleAttribute(const std::string& example, const ExamplesT&... examples)
            : ExampleAttribute(examples...)
        {
            m_examples.push_front(example);
        }
    private:
        ExampleAttribute() {}
        std::list<std::string> m_examples;
    };
}}}}