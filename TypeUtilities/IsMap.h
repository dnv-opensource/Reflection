#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
	template<typename TestType>
	struct IsMapImpl {
	public:
		template<typename KeyT, typename T, typename LessT, typename AllocT> // detect regular operator()
		static char test(const std::map<KeyT, T, LessT, AllocT>&);

		// worst match
		static char(&test(...))[2];

		static const bool value = (sizeof(test(std::declval<TestType>())) == sizeof(char));
	};

	template<typename T>
	constexpr bool IsMap = IsMapImpl<T>::value;

	template<>
	constexpr bool IsMap<void> = false;

	template<typename TestType>
	struct IsMultiMapImpl {
	public:
		template<typename KeyT, typename T, typename LessT, typename AllocT> // detect regular operator()
		static char test(const std::multimap<KeyT, T, LessT, AllocT>&);

		// worst match
		static char(&test(...))[2];

		static const bool value = (sizeof(test(std::declval<TestType>())) == sizeof(char));
	};

	template<typename T>
	constexpr bool IsMultiMap = IsMultiMapImpl<T>::value;

	template<>
	constexpr bool IsMultiMap<void> = false;
}}}