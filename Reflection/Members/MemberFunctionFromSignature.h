//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#	include "Modifier.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    template<typename Signature, typename Modifier = ModifierNamespace::NoModifier>
    struct MemberFunctionFromSignature;

    template<typename R, typename... Args>
    struct MemberFunctionFromSignature<R(Args...)> 
    {
        template<typename ClassT>
        struct Rebind 
        {
            typedef R (ClassT::*Type)(Args...);
        };
    };
    template<typename R, typename... Args>
    struct MemberFunctionFromSignature<R(Args...), ModifierNamespace::Const>
    {
        template<typename ClassT>
        struct Rebind 
        {
            typedef R (ClassT::*Type)(Args...) const;
        };
    };
}}}}
