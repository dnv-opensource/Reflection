#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Operators {
    template<typename T>
    struct NakedType {
        typedef T type;
    };

    template<typename T>
    struct NakedType<const T&> {
        typedef T type;
    };

    template<typename T>
    struct NakedType<T&> {
        typedef T type;
    };

    template<typename T>
    struct NakedType<T&&> {
        typedef T type;
    };
}}}