#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Signature>
    struct IsMember
    {
        static const bool value = false;
    };

    template<typename ReturnTypeT,typename ClassT>
    struct IsMember<ReturnTypeT (ClassT::*)>
    {
        static const bool value = true;
    };

    template<typename ReturnTypeT, typename Class, typename... Args>
    struct IsMember<ReturnTypeT(Class::*)(Args...)>
    {
        static const bool value = true;
    };

    template<typename ReturnTypeT, typename Class, typename... Args>
    struct IsMember<ReturnTypeT(Class::*)(Args...) const>
    {
        static const bool value = true;
    };

}}}
