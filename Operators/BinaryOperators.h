#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "BinaryOperatorResult.h"
#include "Tags.h"
#include "NakedType.h"

namespace DNVS {namespace MoFa {namespace Operators {
    //Define all the binary operators (+,-,*,/,== etc) as generic operators.
    //They will only be enabled if the input arguments share a common domain.
    //We use the domain to compute the result type of the expression + to construct this result type.
    namespace OperatorNamespace {
//         template<typename Lhs,typename Rhs>
//         typename std::enable_if<
//             IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
//             typename BinaryOperatorResult<Tags::ShiftLeft,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
//         >::type
//         operator <<(Lhs&& lhs,Rhs&& rhs)
//         {
//             return BinaryOperatorResult<Tags::ShiftLeft,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
//         }
// 
//         template<typename Lhs,typename Rhs>
//         typename std::enable_if<
//             IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
//             typename BinaryOperatorResult<Tags::ShiftRight,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
//         >::type
//         operator >>(Lhs&& lhs,Rhs&& rhs)
//         {
//             return BinaryOperatorResult<Tags::ShiftRight,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
//         }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Multiplies,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator *(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Multiplies,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Divides,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator /(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Divides,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Modulus,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator %(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Modulus,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Plus,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator +(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Plus,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Minus,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator -(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Minus,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Less,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator <(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Less,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::Greater,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator >(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::Greater,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::LessEqual,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator <=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::LessEqual,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::GreaterEqual,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator >=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::GreaterEqual,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::EqualTo,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator ==(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::EqualTo,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::NotEqualTo,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator !=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::NotEqualTo,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::LogicalOr,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator ||(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::LogicalOr,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::LogicalAnd,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator &&(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::LogicalAnd,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::BitwiseAnd,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator &(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::BitwiseAnd,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::BitwiseOr,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator |(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::BitwiseOr,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::BitwiseXor,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator ^(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::BitwiseXor,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::ShiftLeftAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator <<=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::ShiftLeftAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::ShiftRightAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator >>=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::ShiftRightAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::MultipliesAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator *=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::MultipliesAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::DividesAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator /=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::DividesAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::ModulusAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator %=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::ModulusAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::PlusAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator +=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::PlusAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::MinusAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator -=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::MinusAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::BitwiseAndAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator &=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::BitwiseAndAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::BitwiseOrAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator |=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::BitwiseOrAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

        template<typename Lhs,typename Rhs>
        std::enable_if_t<
            IsBinaryDomain<typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::value,
            typename BinaryOperatorResult<Tags::BitwiseXorAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::type
        >
        operator ^=(Lhs&& lhs,Rhs&& rhs)
        {
            return BinaryOperatorResult<Tags::BitwiseXorAssign,typename NakedType<Lhs&&>::type,typename NakedType<Rhs&&>::type>::BuildExpression(lhs,rhs);
        }

    }

}}}
