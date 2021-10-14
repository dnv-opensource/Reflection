#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
	template<typename TestType>
	struct IsVectorImpl {
	public:
		template<typename T, typename AllocT> // detect regular operator()
		static char test(const std::vector<T, AllocT>&);

		// worst match
		static char(&test(...))[2];

		static const bool value = (sizeof(test(std::declval<TestType>())) == sizeof(char));
	};

	template<typename T>
	constexpr bool IsVector = IsVectorImpl<T>::value;

	template<>
	constexpr bool IsVector<void> = false;
}}}