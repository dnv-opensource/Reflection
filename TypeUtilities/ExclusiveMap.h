#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    //This set contains an element T once. If it is inserted twice, it is erased from the set.
    template<typename Key, typename Value, typename LessT = std::less<Key>>
    class ExclusiveMap : public std::map<Key, Value, LessT>
    {
    public:
        _Pairib insert(const value_type& _Val)
        {
            auto result = std::map<Key, Value, LessT>::insert(_Val);
            if (result.second == false)
            {
                erase(_Val.first);
                return std::make_pair(end(), false);
            }
            return result;
        }
    };
}}}