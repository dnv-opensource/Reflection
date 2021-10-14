#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <tuple>
#include <type_traits>
namespace DNVS {namespace MoFa {namespace TypeUtilities {

    template <class T, class TupleT, std::size_t... I>
    constexpr decltype(auto) TupleConstructImpl(TupleT&& t, std::index_sequence<I...>)
    {
        return new T(std::get<I>(std::forward<TupleT>(t))...);
    }

    template<typename T, typename TupleT>
    constexpr decltype(auto) TupleConstruct(TupleT&& tuple)
    {
        return TupleConstructImpl<T>(std::forward<TupleT>(tuple), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<TupleT>>>{});
    }

    template<typename T, typename TupleT>
    constexpr decltype(auto) TupleMakeShared(TupleT&& tuple)
    {
        return std::shared_ptr<T>(TupleConstructImpl<T>(std::forward<TupleT>(tuple), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<TupleT>>>{}));
    }

}}}
