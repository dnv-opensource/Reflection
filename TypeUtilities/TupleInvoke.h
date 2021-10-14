#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <tuple>
#include <type_traits>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template <class Fn, class TupleT, std::size_t... I>
    constexpr decltype(auto) TupleInvokeImpl(Fn&& f, TupleT&& t, std::index_sequence<I...>)
    {
        return std::invoke(std::forward<Fn>(f), std::get<I>(std::forward<TupleT>(t))...);
    }

    template<typename Fn, typename TupleT>
    constexpr decltype(auto) TupleInvoke(Fn&& fn, TupleT&& tuple)
    {
        return TupleInvokeImpl(std::forward<Fn>(fn), std::forward<TupleT>(tuple), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<TupleT>>>{});
    }

    template<typename Fn, typename TupleT>
    struct TupleInvokeUtility
    {
        using ReturnType = decltype(TupleInvoke(std::declval<Fn>(),std::declval<TupleT>()));
    };

}}}
