#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionTraitsVariadic.h"
#include "IsCallable.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Signature>
    struct FunctionTraits : public FunctionTraitsVariadic<Signature> {};

    template<typename Signature, typename U, typename EnableIf = void>
    struct HasReturnTypeImpl {
        static const bool value = std::is_same_v<typename FunctionTraits<Signature>::ReturnType, U>;
    };
    template<typename Signature, typename U>
    struct HasReturnTypeImpl<Signature, U,std::enable_if_t<!IsCallable<Signature>>>
    {
        static const bool value = false;
    };
    template<typename Signature, typename U>
    constexpr bool HasReturnType = HasReturnTypeImpl<Signature, U>::value;
    
    struct AnyType {};
    template<typename A,typename B>
    constexpr bool IsConvertible = std::is_convertible_v<A, B>;

    template<typename A>
    constexpr bool IsConvertible<A,AnyType> = true;

    template<typename Signature, typename WantedSignature, typename EnableIf = void>
    struct HasConvertibleSignatureImpl {
        template<typename Tuple1,typename Tuple2, typename EnableIf = void>
        struct HaveConvertibleArguments
        {
            static const bool value = false;
        };
        template<>
        struct HaveConvertibleArguments<std::tuple<>, std::tuple<>>
        {
            static const bool value = true;
        };
        template<typename T1, typename... Args1, typename T2, typename... Args2>
        struct HaveConvertibleArguments<std::tuple<T1, Args1...>, std::tuple<T2, Args2...>, std::enable_if_t<sizeof...(Args1) == sizeof...(Args2)>>
        {
            static const bool value = IsConvertible<T1, T2> && HaveConvertibleArguments<std::tuple<Args1...>, std::tuple<Args2...>>::value;
        };
        static const bool value = IsConvertible<typename FunctionTraits<Signature>::ReturnType, typename FunctionTraits<WantedSignature>::ReturnType> &&
            HaveConvertibleArguments<typename FunctionTraits<Signature>::Arguments, typename FunctionTraits<WantedSignature>::Arguments>::value;
    };

    template<typename Signature, typename WantedSignature>
    struct HasConvertibleSignatureImpl<Signature, WantedSignature, std::enable_if_t<!IsCallable<Signature>>>
    {
        static const bool value = false;
    };

    template<typename Signature, typename WantedSignature>
    constexpr bool HasConvertibleSignature = HasConvertibleSignatureImpl<Signature, WantedSignature>::value;
}}}

