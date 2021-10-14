#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <tuple>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Fn, typename T, typename... A>
    void TupleForEachExpanded(Fn&& fn, T&& t, A&&... rest)
    {
        fn(std::forward<T>(t));
        TupleForEachExpanded(std::forward<Fn>(fn), std::forward<A>(rest)...);
    }
    template<typename Fn>
    void TupleForEachExpanded(Fn&& fn)
    {}

    template<typename TupleT, typename Fn, size_t... Indices>
    void TupleForEachImpl(TupleT&& tuple, Fn&& fn, const std::index_sequence<Indices...>&)
    {
        TupleForEachExpanded(std::forward<Fn>(fn), std::get<Indices>(std::forward<TupleT>(tuple))...);
    }

    template<typename TupleT,typename Fn>
    void TupleForEach(TupleT&& tuple,Fn&& fn)
    {
        TupleForEachImpl(std::forward<TupleT>(tuple), std::forward<Fn>(fn), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<TupleT>>>());
    }

}}}
