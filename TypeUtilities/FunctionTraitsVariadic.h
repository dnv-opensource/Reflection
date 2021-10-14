#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <tuple>
#include "TypeUtilities/TypeList.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Lambda, typename Signature>
    struct FunctionTraitsVariadicLambda;

    template<typename Signature>
    struct FunctionTraitsVariadic : public FunctionTraitsVariadicLambda<Signature, decltype(&Signature::operator())> {};

    template<typename... Args>
    struct FunctionTraitsSpecializationImpl {};
    template<typename A0>
    struct FunctionTraitsSpecializationImpl<A0>
    {
        typedef A0 Argument0;
    };
    template<typename A0, typename A1>
    struct FunctionTraitsSpecializationImpl<A0, A1>
        : FunctionTraitsSpecializationImpl<A0>
    {
        typedef A1 Argument1;
    };
    template<typename A0, typename A1, typename A2>
    struct FunctionTraitsSpecializationImpl<A0, A1, A2>
        : FunctionTraitsSpecializationImpl<A0, A1>
    {
        typedef A2 Argument2;
    };
    template<typename A0, typename A1, typename A2, typename A3, typename... Args>
    struct FunctionTraitsSpecializationImpl<A0, A1, A2, A3, Args...>
        : FunctionTraitsSpecializationImpl<A0, A1, A2>
    {
        typedef A3 Argument3;
    };

    template<typename ReturnTypeT, typename... Args>
    struct FunctionTraitsImpl : FunctionTraitsSpecializationImpl<Args...>
    {
        static const size_t Arity = sizeof...(Args);
        typedef ReturnTypeT ReturnType;
        typedef std::tuple<Args...> Arguments;
        template<typename VisitorT>
        static void Visit(VisitorT& visitor)
        {
            VisitorImpl(visitor, TypeList<Args...>());
        }
    private:
        template<typename VisitorT>
        static void VisitorImpl(VisitorT& visitor, TypeList<>)
        {}

        template<typename VisitorT, typename T, typename... LocalArgs>
        static void VisitorImpl(VisitorT& visitor, TypeList<T, LocalArgs...>)
        {
            visitor.Visit<T, sizeof...(Args)-sizeof...(LocalArgs)-1>();
            VisitorImpl(visitor, TypeList<LocalArgs...>());
        }
    };

    template<typename ReturnTypeT,typename ClassT>
    struct FunctionTraitsVariadic<ReturnTypeT (ClassT::*)> : FunctionTraitsImpl<ReturnTypeT&, ClassT*>
    {
        typedef ReturnTypeT(ClassT::*StorageType);
    };

    template<typename ReturnTypeT, typename... Args>
    struct FunctionTraitsVariadic<ReturnTypeT(*)(Args...)> : FunctionTraitsImpl<ReturnTypeT, Args...>
    {
        typedef ReturnTypeT(*StorageType)(Args...);
    };

    template<typename ReturnTypeT, typename... Args>
    struct FunctionTraitsVariadic<ReturnTypeT(Args...)> : FunctionTraitsImpl<ReturnTypeT, Args...>
    {
        typedef ReturnTypeT(*StorageType)(Args...);
    };

    template<typename ReturnTypeT, typename Class, typename... Args>
    struct FunctionTraitsVariadic<ReturnTypeT(Class::*)(Args...)> : FunctionTraitsImpl<ReturnTypeT, Class*, Args...>
    {
        typedef ReturnTypeT(Class::*StorageType)(Args...);
    };

    template<typename ReturnTypeT, typename Class, typename... Args>
    struct FunctionTraitsVariadic<ReturnTypeT(Class::*)(Args...) const> : FunctionTraitsImpl<ReturnTypeT, const Class*, Args...>
    {
        typedef ReturnTypeT(Class::*StorageType)(Args...) const;
    };

    template<typename Lambda, typename ReturnTypeT, typename Class, typename... Args>
    struct FunctionTraitsVariadicLambda<Lambda, ReturnTypeT(Class::*)(Args...) const> : FunctionTraitsImpl<ReturnTypeT, Args...>
    {
        typedef Lambda StorageType;
    };

    template<typename ReturnTypeT, template<typename T> typename FunctionT, typename... Args>
    struct FunctionTraitsVariadic<FunctionT<ReturnTypeT(Args...)>> : FunctionTraitsImpl<ReturnTypeT, Args...>
    {
        typedef FunctionT<ReturnTypeT(Args...)> StorageType;
    };

}}}

