#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <type_traits>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Classes {

    template<typename... Bases>
    class Public
    {
    };

    template<typename T>
    struct IsPublic : std::false_type
    {};

    template<typename... Bases>
    struct IsPublic<Public<Bases...>> : std::true_type
    {};


}}}}