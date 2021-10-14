#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename ContainerT,typename ValueT, typename EnableIf = void>
    struct IsContainerOf : std::integral_constant<bool, false>
    {};

    template<typename ContainerT, typename ValueT>
    struct IsContainerOf<ContainerT, ValueT, typename std::enable_if<std::is_convertible<typename ContainerT::value_type, ValueT>::value>::type>
        : std::integral_constant<bool, true>
    {};
}}}
