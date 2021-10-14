#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt#include <Operators\Tags.h>
#include "Operators\ResultOf.h"

namespace DNVS {namespace MoFa {namespace Operators {
    //For any given operator tag, you can evaluate the expression using an Invoker. 
    //Invoker<Tags::Plus>::Invoke(5,9.3); will return the result of 5+9.3.
    //This is useful when writing a generic class to handle all binary operators in a consistent manner.

    template<typename Tag>
    struct Invoker;

    template<>
    struct Invoker<Tags::UnaryPlus>
    {
        template<typename A0>
        static typename ResultOf<Tags::UnaryPlus(A0&&)>::result_type Invoke(A0&& a0) {
            return +a0;
        }
    };

    template<>
    struct Invoker<Tags::Negate>
    {
        template<typename A0>
        static typename ResultOf<Tags::Negate(A0&&)>::result_type Invoke(A0&& a0) {
            return -a0;
        }
    };

    template<>
    struct Invoker<Tags::Dereference>
    {
        template<typename A0>
        static typename ResultOf<Tags::Dereference(A0&&)>::result_type Invoke(A0&& a0) {
            return *a0;
        }
    };

    template<>
    struct Invoker<Tags::Complement>
    {
        template<typename A0>
        static typename ResultOf<Tags::Complement(A0&&)>::result_type Invoke(A0&& a0) {
            return ~a0;
        }
    };

    template<>
    struct Invoker<Tags::AddressOf>
    {
        template<typename A0>
        static typename ResultOf<Tags::AddressOf(A0&&)>::result_type Invoke(A0&& a0) {
            return &a0;
        }
    };

    template<>
    struct Invoker<Tags::LogicalNot>
    {
        template<typename A0>
        static typename ResultOf<Tags::LogicalNot(A0&&)>::result_type Invoke(A0&& a0) {
            return !a0;
        }
    };

    template<>
    struct Invoker<Tags::PreInc>
    {
        template<typename A0>
        static typename ResultOf<Tags::PreInc(A0&&)>::result_type Invoke(A0&& a0) {
            return ++a0;
        }
    };

    template<>
    struct Invoker<Tags::PreDec>
    {
        template<typename A0>
        static typename ResultOf<Tags::PreDec(A0&&)>::result_type Invoke(A0&& a0) {
            return --a0;
        }
    };

    template<>
    struct Invoker<Tags::PostInc>
    {
        template<typename A0>
        static typename ResultOf<Tags::PostInc(A0&&)>::result_type Invoke(A0&& a0) {
            return a0++;
        }
    };

    template<>
    struct Invoker<Tags::PostDec>
    {
        template<typename A0>
        static typename ResultOf<Tags::PostDec(A0&&)>::result_type Invoke(A0&& a0) {
            return a0--;
        }
    };

    template<>
    struct Invoker<Tags::ShiftLeft>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::ShiftLeft(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0<<a1;
        }
    };

    template<>
    struct Invoker<Tags::ShiftRight>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::ShiftRight(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0>>a1;
        }
    };

    template<>
    struct Invoker<Tags::Multiplies>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Multiplies(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0*a1;
        }
    };

    template<>
    struct Invoker<Tags::Divides>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Divides(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0/a1;
        }
    };

    template<>
    struct Invoker<Tags::Modulus>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Modulus(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0%a1;
        }
    };

    template<>
    struct Invoker<Tags::Plus>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Plus(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0+a1;
        }
    };

    template<>
    struct Invoker<Tags::Minus>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Minus(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0-a1;
        }
    };

    template<>
    struct Invoker<Tags::Less>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Less(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0<a1;
        }
    };

    template<>
    struct Invoker<Tags::Greater>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Greater(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0>a1;
        }
    };

    template<>
    struct Invoker<Tags::LessEqual>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::LessEqual(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0<=a1;
        }
    };

    template<>
    struct Invoker<Tags::GreaterEqual>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::GreaterEqual(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0>=a1;
        }
    };

    template<>
    struct Invoker<Tags::EqualTo>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::EqualTo(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0==a1;
        }
    };

    template<>
    struct Invoker<Tags::NotEqualTo>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::NotEqualTo(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0!=a1;
        }
    };

    template<>
    struct Invoker<Tags::LogicalOr>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::LogicalOr(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0||a1;
        }
    };

    template<>
    struct Invoker<Tags::LogicalAnd>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::LogicalAnd(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0&&a1;
        }
    };

    template<>
    struct Invoker<Tags::BitwiseAnd>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::BitwiseAnd(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0&a1;
        }
    };

    template<>
    struct Invoker<Tags::BitwiseOr>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::BitwiseOr(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0|a1;
        }
    };

    template<>
    struct Invoker<Tags::BitwiseXor>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::BitwiseXor(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0^a1;
        }
    };

    template<>
    struct Invoker<Tags::Assign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Assign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0=a1;
        }
    };

    template<>
    struct Invoker<Tags::ShiftLeftAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::ShiftLeftAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0<<=a1;
        }
    };

    template<>
    struct Invoker<Tags::ShiftRightAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::ShiftRightAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0>>=a1;
        }
    };

    template<>
    struct Invoker<Tags::MultipliesAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::MultipliesAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0*=a1;
        }
    };

    template<>
    struct Invoker<Tags::DividesAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::DividesAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0/=a1;
        }
    };

    template<>
    struct Invoker<Tags::ModulusAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::ModulusAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0%=a1;
        }
    };

    template<>
    struct Invoker<Tags::PlusAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::PlusAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0+=a1;
        }
    };

    template<>
    struct Invoker<Tags::MinusAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::MinusAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0-=a1;
        }
    };

    template<>
    struct Invoker<Tags::BitwiseAndAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::BitwiseAndAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0&=a1;
        }
    };

    template<>
    struct Invoker<Tags::BitwiseOrAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::BitwiseOrAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0|=a1;
        }
    };

    template<>
    struct Invoker<Tags::BitwiseXorAssign>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::BitwiseXorAssign(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0^=a1;
        }
    };

    template<>
    struct Invoker<Tags::Subscript>
    {
        template<typename A0,typename A1>
        static typename ResultOf<Tags::Subscript(A0&&,A1&&)>::result_type Invoke(A0&& a0,A1&& a1) {
            return a0[a1];
        }
    };
}}}
