#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Operators\Tags.h>
#include "PrecedenceEnum.h"

namespace DNVS { namespace MoFa { namespace Operators {
    template<typename Tag>
    struct Precedence;

    template<>
    struct Precedence<Tags::UnaryPlus>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::Negate>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::Dereference>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::Complement>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::AddressOf>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::LogicalNot>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::PreInc>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::PreDec>
    {
        static const PrecedenceEnum value = SuffixPrecedence;
    };

    template<>
    struct Precedence<Tags::PostInc>
    {
        static const PrecedenceEnum value = ScopePrecedence;
    };

    template<>
    struct Precedence<Tags::PostDec>
    {
        static const PrecedenceEnum value = ScopePrecedence;
    };

    template<>
    struct Precedence<Tags::ShiftLeft>
    {
        static const PrecedenceEnum value = BitwiseShiftPrecedence;        
    };

    template<>
    struct Precedence<Tags::ShiftRight>
    {
        static const PrecedenceEnum value = BitwiseShiftPrecedence;        
    };

    template<>
    struct Precedence<Tags::Multiplies>
    {
        static const PrecedenceEnum value = MultiplicationPrecedence;        
    };

    template<>
    struct Precedence<Tags::Divides>
    {
        static const PrecedenceEnum value = MultiplicationPrecedence;        
    };

    template<>
    struct Precedence<Tags::Modulus>
    {
        static const PrecedenceEnum value = MultiplicationPrecedence;        
    };

    template<>
    struct Precedence<Tags::Plus>
    {
        static const PrecedenceEnum value = AdditionPrecedence;        
    };

    template<>
    struct Precedence<Tags::Minus>
    {
        static const PrecedenceEnum value = AdditionPrecedence;        
    };

    template<>
    struct Precedence<Tags::Less>
    {
        static const PrecedenceEnum value = RelationalOperatorPrecedence;        
    };

    template<>
    struct Precedence<Tags::Greater>
    {
        static const PrecedenceEnum value = RelationalOperatorPrecedence;        
    };

    template<>
    struct Precedence<Tags::LessEqual>
    {
        static const PrecedenceEnum value = RelationalOperatorPrecedence;        
    };

    template<>
    struct Precedence<Tags::GreaterEqual>
    {
        static const PrecedenceEnum value = RelationalOperatorPrecedence;        
    };

    template<>
    struct Precedence<Tags::EqualTo>
    {
        static const PrecedenceEnum value = RelationalEqualityPrecedence;        
    };

    template<>
    struct Precedence<Tags::NotEqualTo>
    {
        static const PrecedenceEnum value = RelationalEqualityPrecedence;        
    };

    template<>
    struct Precedence<Tags::LogicalOr>
    {
        static const PrecedenceEnum value = LogicalOrPrecedence;        
    };

    template<>
    struct Precedence<Tags::LogicalAnd>
    {
        static const PrecedenceEnum value = LogicalAndPrecedence;
    };

    template<>
    struct Precedence<Tags::BitwiseAnd>
    {
        static const PrecedenceEnum value = BitwiseAndPrecedence;
    };

    template<>
    struct Precedence<Tags::BitwiseOr>
    {
        static const PrecedenceEnum value = BitwiseOrPrecedence;
    };

    template<>
    struct Precedence<Tags::BitwiseXor>
    {
        static const PrecedenceEnum value = BitwiseXorPrecedence;
    };

    template<>
    struct Precedence<Tags::Assign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::ShiftLeftAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::ShiftRightAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::MultipliesAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::DividesAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::ModulusAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::PlusAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::MinusAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::BitwiseAndAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::BitwiseOrAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::BitwiseXorAssign>
    {
        static const PrecedenceEnum value = AssignmentConditionPrecedence;
    };

    template<>
    struct Precedence<Tags::Subscript>
    {
        static const PrecedenceEnum value = ScopePrecedence;
    };
}}}