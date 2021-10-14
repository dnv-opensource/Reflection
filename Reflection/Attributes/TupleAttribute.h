#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class TupleAttribute {
    public:
		template<typename TupleT>
        TupleAttribute(const Types::DecoratedTypeInfo& tupleType, const TupleT* args)
			: m_tupleType(tupleType)
			, m_argumentTypes(std::tuple_size_v<TupleT>)
		{
			InitializeArguments<TupleT>(std::integral_constant<size_t, std::tuple_size<TupleT>::value-1>());
		}
		const std::vector<Types::DecoratedTypeInfo>& GetArgumentTypes() const { return m_argumentTypes; }
		const Types::DecoratedTypeInfo& GetTupleType() const { return m_tupleType; }
    private:

		template<typename TupleT>
		void InitializeArguments(std::integral_constant<size_t, 0>)
		{
			m_argumentTypes[0] = Types::TypeId<std::tuple_element_t<0, TupleT>>();
		}

		template<typename TupleT, size_t Index>
		void InitializeArguments(std::integral_constant<size_t, Index>)
		{
			m_argumentTypes[Index] = Types::TypeId<std::tuple_element_t<Index, TupleT>>();
			InitializeArguments<TupleT>(std::integral_constant<size_t, Index-1>());
		}
		Types::DecoratedTypeInfo m_tupleType;
		std::vector<Types::DecoratedTypeInfo> m_argumentTypes;
    };
}}}}
