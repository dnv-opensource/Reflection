#pragma once 
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities\FunctionTraits.h"
#include "TypeUtilities\IsMember.h"

template<bool>
struct ClassTypeGetter 
{
    template<typename Fn>
    static const type_info& Get(Fn fn)
    {
        return typeid(DNVS::MoFa::TypeUtilities::FunctionTraits<Fn>::Argument0);
    }
};

template<>
struct ClassTypeGetter<false>
{
    template<typename Fn>
    static const type_info& Get(Fn fn)
    {
        return typeid(void);
    }
};