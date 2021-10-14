#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <set>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    //This set contains an element T once. If it is inserted twice, it is erased from the set.
    template<typename T, typename LessT = std::less<T>>
    class ExclusiveSet : public std::set<T, LessT>
    {
    public:
        _Pairib insert(const value_type& _Val)
        {
            auto result = std::set<T, LessT>::insert(_Val);
            if (result.second == false)
            {
                erase(_Val);
                return std::make_pair(end(), false);
            }
            return result;
        }
    };
}}}