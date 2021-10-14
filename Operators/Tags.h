#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Operators { namespace Tags {
    //Define a class that corresponds to each operator in order to allow treat different operators in a consistent manner using templates.
    //You can e.g. pass an operator tag as a template argument.

        /// Tag type for the unary + operator.
        struct UnaryPlus {};

        /// Tag type for the unary - operator.
        struct Negate {};

        /// Tag type for the unary * operator.
        struct Dereference {};

        /// Tag type for the unary ~ operator.
        struct Complement {};

        /// Tag type for the unary & operator.
        struct AddressOf {};

        /// Tag type for the unary ! operator.
        struct LogicalNot {};

        /// Tag type for the unary prefix ++ operator.
        struct PreInc {};

        /// Tag type for the unary prefix -- operator.
        struct PreDec {};

        /// Tag type for the unary postfix ++ operator.
        struct PostInc {};

        /// Tag type for the unary postfix -- operator.
        struct PostDec {};

        /// Tag type for the binary \<\< operator.
        struct ShiftLeft {};

        /// Tag type for the binary \>\> operator.
        struct ShiftRight {};

        /// Tag type for the binary * operator.
        struct Multiplies {};

        /// Tag type for the binary / operator.
        struct Divides {};

        /// Tag type for the binary % operator.
        struct Modulus {};

        /// Tag type for the binary + operator.
        struct Plus {};

        /// Tag type for the binary - operator.
        struct Minus {};

        /// Tag type for the binary \< operator.
        struct Less {};

        /// Tag type for the binary \> operator.
        struct Greater {};

        /// Tag type for the binary \<= operator.
        struct LessEqual {};

        /// Tag type for the binary \>= operator.
        struct GreaterEqual {};

        /// Tag type for the binary == operator.
        struct EqualTo {};

        /// Tag type for the binary != operator.
        struct NotEqualTo {};

        /// Tag type for the binary || operator.
        struct LogicalOr {};

        /// Tag type for the binary && operator.
        struct LogicalAnd {};

        /// Tag type for the binary & operator.
        struct BitwiseAnd {};

        /// Tag type for the binary | operator.
        struct BitwiseOr {};

        /// Tag type for the binary ^ operator.
        struct BitwiseXor {};

        /// Tag type for the binary , operator.
        struct Comma {};

        /// Tag type for the binary ->* operator.
        struct MemPtr {};

        /// Tag type for the binary = operator.
        struct Assign {};

        /// Tag type for the binary \<\<= operator.
        struct ShiftLeftAssign {};

        /// Tag type for the binary \>\>= operator.
        struct ShiftRightAssign {};

        /// Tag type for the binary *= operator.
        struct MultipliesAssign {};

        /// Tag type for the binary /= operator.
        struct DividesAssign {};

        /// Tag type for the binary %= operator.
        struct ModulusAssign {};

        /// Tag type for the binary += operator.
        struct PlusAssign {};

        /// Tag type for the binary -= operator.
        struct MinusAssign {};

        /// Tag type for the binary &= operator.
        struct BitwiseAndAssign {};

        /// Tag type for the binary |= operator.
        struct BitwiseOrAssign {};

        /// Tag type for the binary ^= operator.
        struct BitwiseXorAssign {};

        /// Tag type for the binary subscript operator.
        struct Subscript {};
}}}}
