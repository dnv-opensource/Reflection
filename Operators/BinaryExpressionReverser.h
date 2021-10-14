#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "BinaryOperatorResult.h"
#include "UnaryOperatorResult.h"
namespace DNVS {namespace MoFa {namespace Operators {
    template<typename Tag, typename A0, typename A1>
    struct BinaryExpressionReverser 
    {
        static const bool value = false;
    };

    template<typename A0, typename A1>
    struct BinaryExpressionReverser<Tags::Plus, A0, A1>
    {
        static const bool value = true;
        typedef BinaryOperatorResult<Tags::Plus,typename NakedType<A1>::type,typename NakedType<A0>::type> ResultType;
        typedef typename ResultType::type type;
        static type BuildExpression(A0&& a0, A1&& a1)
        {
            return ResultType::BuildExpression(a1, a0);
        }
    };

    template<typename A0, typename A1>
    struct BinaryExpressionReverser<Tags::Minus, A0, A1>
    {
        static const bool value = true;
        typedef UnaryOperatorResult<Tags::Negate, typename NakedType<A1>::type> NegateResultType;
        typedef BinaryOperatorResult<Tags::Plus, typename NegateResultType::type, typename NakedType<A0>::type> ResultType;
        typedef typename ResultType::type type;
        static type BuildExpression(A0&& a0, A1&& a1)
        {
            return ResultType::BuildExpression(NegateResultType::BuildExpression(a1), a0);
        }
    };

    template<typename A0, typename A1>
    struct BinaryExpressionReverser<Tags::Multiplies, A0, A1>
    {
        static const bool value = true;
        typedef BinaryOperatorResult<Tags::Multiplies,typename NakedType<A1>::type,typename NakedType<A0>::type> ResultType;
        typedef typename ResultType::type type;
        static type BuildExpression(A0&& a0, A1&& a1)
        {
            return ResultType::BuildExpression(a1, a0);
        }
    };

}}}