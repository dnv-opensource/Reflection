#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <set>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
	template<typename TestType>
	struct IsSetImpl {
	public:
		template<typename T, typename LessT, typename AllocT> // detect regular operator()
		static char test(const std::set<T, LessT, AllocT>&);

		// worst match
		static char(&test(...))[2];

		static const bool value = (sizeof(test(std::declval<TestType>())) == sizeof(char));
	};

	template<typename T>
	constexpr bool IsSet = IsSetImpl<T>::value;

	template<>
	constexpr bool IsSet<void> = false;

	template<typename TestType>
	struct IsMultiSetImpl {
	public:
		template<typename T, typename LessT, typename AllocT> // detect regular operator()
		static char test(const std::multiset<T, LessT, AllocT>&);

		// worst match
		static char(&test(...))[2];

		static const bool value = (sizeof(test(std::declval<TestType>())) == sizeof(char));
	};

	template<typename T>
	constexpr bool IsMultiSet = IsMultiSetImpl<T>::value;

	template<>
	constexpr bool IsMultiSet<void> = false;
}}}