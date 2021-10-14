#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
        template<typename Signature>
        struct Invoker;

        template<typename Signature, typename ArgumentsT>
        typename Invoker<Signature>::ReturnType Invoke(Signature function, ArgumentsT& args)
        {
            return Invoker<Signature>::Invoke(function, args);
        }

        template<typename ReturnTypeT, typename ClassT>
        struct Invoker<ReturnTypeT (ClassT::*)>
        {
            typedef ReturnTypeT& ReturnType;
            typedef ReturnTypeT (ClassT::*Signature);
            template<typename ArgumentsT>
            static ReturnType Invoke(Signature member, ArgumentsT& args) 
            {
                return args.Get<ClassT*, 0>()->*member;
            }
        };

        template<typename ReturnTypeT, typename A0>
        struct Invoker<ReturnTypeT (*)(A0)>
        {
            typedef ReturnTypeT ReturnType;
            typedef ReturnType (*Signature)(A0);
            template<typename ArgumentsT>
            static ReturnType Invoke(Signature function, ArgumentsT& args) 
            {
                return (*function)(args.Get<A0, 0>());
            }
        };

        template<typename ReturnTypeT, typename Class, typename A0, typename A1>
        struct Invoker<ReturnTypeT (Class::*)(A0, A1)>
        {
            typedef ReturnTypeT ReturnType;
            typedef ReturnType (Class::*Signature)(A0, A1);
            template<typename ArgumentsT>
            static ReturnType Invoke(Signature function, ArgumentsT& args) 
            {
                return (args.Get<Class*, 0>()->*function)(args.Get<A0, 1>(), args.Get<A1, 2>());
            }
        };

}}}}
