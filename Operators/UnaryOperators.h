#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnaryOperatorResult.h"
#include "Tags.h"
#include "NakedType.h"

namespace DNVS {namespace MoFa {namespace Operators {
    //Define all the unary operators (!,~,- etc) as generic operators.
    //They will only be enabled if the input argument has a domain.
    //We use the domain to compute the result type of the expression + to construct this result type.
    namespace OperatorNamespace {
        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::UnaryPlus,typename NakedType<SubjectType&&>::type>::type
        >
        operator +(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::UnaryPlus,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::Negate,typename NakedType<SubjectType&&>::type>::type
        >
        operator -(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::Negate,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::Dereference,typename NakedType<SubjectType&&>::type>::type
        >
        operator *(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::Dereference,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::Complement,typename NakedType<SubjectType&&>::type>::type
        >
        operator ~(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::Complement,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

//         template<typename SubjectType>
//         typename std::enable_if<
//             IsUnaryDomain<typename NakedType<SubjectType&&>::type>,
//             typename UnaryOperatorResult<Tags::AddressOf,typename NakedType<SubjectType&&>::type>::type
//         >::type
//         operator &(SubjectType&& subject)
//         {
//             return UnaryOperatorResult<Tags::AddressOf,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
//         }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::LogicalNot,typename NakedType<SubjectType&&>::type>::type
        >
        operator !(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::LogicalNot,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::PreInc,typename NakedType<SubjectType&&>::type>::type
        >
        operator ++(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::PreInc,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::PreDec,typename NakedType<SubjectType&&>::type>::type
        >
        operator --(SubjectType&& subject)
        {
            return UnaryOperatorResult<Tags::PreDec,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::PostInc,typename NakedType<SubjectType&&>::type>::type
        >
        operator ++(SubjectType&& subject,int)
        {
            return UnaryOperatorResult<Tags::PostInc,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }

        template<typename SubjectType>
        std::enable_if_t<
            IsUnaryDomain<typename NakedType<SubjectType&&>::type>::value,
            typename UnaryOperatorResult<Tags::PostDec,typename NakedType<SubjectType&&>::type>::type
        >
        operator --(SubjectType&& subject,int)
        {
            return UnaryOperatorResult<Tags::PostDec,typename NakedType<SubjectType&&>::type>::BuildExpression(subject);
        }
    }
}}}
