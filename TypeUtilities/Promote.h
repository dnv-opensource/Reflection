#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename T, typename EnableIf = void>
    struct Promote
    {
        typedef T type;
    };

    template<typename T>
    struct Promote<const T>
    {
        typedef typename std::add_const<typename Promote<T>::type>::type type;
    };

    template<typename T>
    struct Promote<volatile T>
    {
        typedef typename std::add_volatile<typename Promote<T>::type>::type type;
    };
    template<>
    struct Promote<float>
    {
        typedef double type;
    };

    template<typename T>
    struct Promote<T,
        typename std::enable_if<
            std::is_integral<T>::value && 
            !std::is_const<T>::value &&
            !std::is_volatile<T>::value
        >::type
    >        
    {
        typedef decltype(+T()) type;
    };
}}}