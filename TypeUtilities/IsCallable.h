#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename T>
    struct HasCallOperatorImpl {
    public:
        template<typename C> // detect regular operator()
        static char test(decltype(&C::operator()));

        template<typename C> // worst match
        static char(&test(...))[2];

        static const bool value = (sizeof(test<T>(nullptr)) == sizeof(char));
    };

    template<typename T>
    constexpr bool HasCallOperator = HasCallOperatorImpl<T>::value;

    template<typename T>
    constexpr bool IsCallable = std::is_function_v<T> || HasCallOperator<T>;

    template<typename R,typename... Args>
    constexpr bool IsCallable<R(*)(Args...)> = true;
}}}