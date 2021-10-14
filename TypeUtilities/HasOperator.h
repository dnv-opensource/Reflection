#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <list>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
	namespace HasBinaryOperatorImpl {
        struct No {};
        template<typename Lhs, typename Rhs>
        No operator==(const Lhs&, const Rhs&);
        template<typename Lhs, typename Rhs>
        No operator!=(const Lhs&, const Rhs&);

        template<typename Lhs, typename Rhs>
        constexpr bool HasEqualTo = std::is_same_v<decltype(std::declval<Lhs>() == std::declval<Rhs>()), bool>;

        template<typename Lhs, typename Rhs>
        constexpr bool HasNotEqualTo = std::is_same_v<decltype(std::declval<Lhs>() != std::declval<Rhs>()), bool>;

	};

    template<typename... Args>
    constexpr bool HasEqualTo = false;

    template<typename Lhs>
    constexpr bool HasEqualTo<Lhs> = HasBinaryOperatorImpl::HasEqualTo<Lhs, Lhs>;

    template<typename Lhs, typename Rhs>
    constexpr bool HasEqualTo<Lhs, Rhs> = HasBinaryOperatorImpl::HasEqualTo<Lhs, Rhs>;

    template<typename... Args>
    constexpr bool HasNotEqualTo = false;

    template<typename Lhs>
    constexpr bool HasNotEqualTo<Lhs> = HasBinaryOperatorImpl::HasNotEqualTo<Lhs, Lhs>;

    template<typename Lhs, typename Rhs>
    constexpr bool HasNotEqualTo<Lhs, Rhs> = HasBinaryOperatorImpl::HasNotEqualTo<Lhs, Rhs>;
}}}