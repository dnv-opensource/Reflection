#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <algorithm>
#include <ppl.h>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename It,typename Fn>
    void ForEachAllowErase(It first, It last, const Fn& fn)
    {
        while(first != last)
        {
            It current = first++;
            fn(*current);
        }
    }
}}}