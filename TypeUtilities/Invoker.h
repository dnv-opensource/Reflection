#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "InvokerImpl.h"
#include <stdexcept>
#include <string>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Lambda, typename Signature>
    struct InvokerLambda;

    template<typename Signature>
    struct Invoker : InvokerLambda<Signature, decltype(&Signature::operator())>
    {
    };

    template<typename Signature,typename ArgumentsT>
    typename Invoker<Signature>::ReturnType Invoke(Signature function,ArgumentsT& args)
    {
        return Invoker<Signature>::Invoke(function,args);
    }

    template<typename ReturnTypeT,typename ClassT>
    struct Invoker<ReturnTypeT (ClassT::*)>
    {
        typedef ReturnTypeT& ReturnType;
        typedef ReturnTypeT (ClassT::*Signature);
        template<typename ArgumentsT>
        static ReturnType Invoke(Signature member,ArgumentsT& args) {
            ClassT* cls=args.Get<ClassT*,0>();
            if (!cls)
                throw std::runtime_error(typeid(ClassT).name() + std::string(" is null"));
            return cls->*member;
        }
    };
    
    template<typename ReturnTypeT, typename... Args>
    struct Invoker<ReturnTypeT(*)(Args...)>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType(*Signature)(Args...);
        template<typename ArgumentsT>
        static ReturnType Invoke(Signature function, ArgumentsT& args) {
            return InvokeImpl(args, function, TypeList<Args...>());
        }
    };
    template<typename ReturnTypeT, typename... Args>
    struct Invoker<ReturnTypeT(Args...)>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType Signature(Args...);
        template<typename ArgumentsT>
        static ReturnType Invoke(Signature function, ArgumentsT& args) {
            return InvokeImpl(args, function, TypeList<Args...>());
        }
    };

    template<typename ReturnTypeT, template<typename T> typename FunctionT, typename ... Args>
    struct Invoker<FunctionT<ReturnTypeT(Args...)> >
    {
        typedef ReturnTypeT ReturnType;
        typedef FunctionT<ReturnTypeT(Args...)> Signature;
        template<typename ArgumentsT>
        static ReturnType Invoke(const Signature& function, ArgumentsT& args) {
            return InvokeImpl(args, function, TypeList<Args...>());
        }
    };

    template<typename Lambda, typename ReturnTypeT, typename Class, typename... Args>
    struct InvokerLambda<Lambda, ReturnTypeT(Class::*)(Args...) const>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType(Signature)(Args...);
        template<typename ArgumentsT>
        static ReturnType Invoke(const Lambda& function, ArgumentsT& args) {
            return InvokeImpl(args, function, TypeList<Args...>());
        }
    };


    template<typename ReturnTypeT, typename Class, typename... Args>
    struct Invoker<ReturnTypeT(Class::*)(Args...)>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType(Class::*Signature)(Args...);
        template<typename ArgumentsT>
        static ReturnType Invoke(Signature function, ArgumentsT& args) {
            auto callback = [function](Class* cls, Args... args) -> ReturnType
            {
                if (!cls)
                    throw std::runtime_error(typeid(Class).name() + std::string(" is null"));
                return (cls->*function)(args...);
            };
            return InvokeImpl(args, callback, TypeList<Class*, Args...>());
        }
    };

    template<typename ReturnTypeT, typename Class, typename... Args>
    struct Invoker<ReturnTypeT(Class::*)(Args...) const>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType(Class::*Signature)(Args...) const;
        template<typename ArgumentsT>
        static ReturnType Invoke(Signature function, ArgumentsT& args) {
            auto callback = [function](const Class* cls, Args... args) -> ReturnType
            {
                if (!cls)
                    throw std::runtime_error(typeid(Class).name() + std::string(" is null"));
                return (cls->*function)(args...);
            };
            return InvokeImpl(args, callback, TypeList<const Class*, Args...>());
        }
    };
}}}
