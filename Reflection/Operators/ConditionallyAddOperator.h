#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "../Classes/Class.h"
#include "DefineOperator.h"
#include "TypeUtilities/HasOperator.h"
#include "This.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Operators {
    template<bool>
    struct ConditionalOperator {
        template<typename Class, typename StorageType, typename OperatorGenerator>
        static auto& Operator(DefineOperator<Class, StorageType>& cls, const OperatorGenerator& op)
        {
            return cls.Operator(op);
        }
    };
    template<>
    struct ConditionalOperator<false> {
        template<typename Class, typename StorageType, typename OperatorGenerator>
        static void Operator(DefineOperator<Class, StorageType>& cls, const OperatorGenerator& op)
        {}
    };
    template<typename Class, typename StorageType, typename Lhs, typename Rhs>
    auto& DefineConditionalOperator(DefineOperator<Class, StorageType>& cls, const ThisOperators::BinaryOperatorGenerator<Lhs, Rhs, DNVS::MoFa::Operators::Tags::EqualTo>& op)
    {
        using NewLhs = typename RebindArgument<Lhs, Class, StorageClass>::Type;
        using NewRhs = typename RebindArgument<Rhs, Class, StorageClass>::Type;
        return ConditionalOperator<TypeUtilities::HasEqualTo<NewLhs, NewRhs>>::Operator(cls, op);
    }
    
}}}}