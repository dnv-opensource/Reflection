#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS { namespace MoFa { namespace TypeUtilities {

    class LessDeref
    {
    public:
        using is_transparent = int;
       
        template<typename U,typename V>
        bool operator()(const U& lhs, const V& rhs) const
        {
            return *lhs < *rhs;
        }
    };
}}}