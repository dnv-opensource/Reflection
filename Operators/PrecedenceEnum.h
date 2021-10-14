#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
namespace DNVS { namespace MoFa { namespace Operators {
    enum PrecedenceEnum {
        ConditionalPrecedence = -1,
        BasePrecedence = 0,
        //:: 	Scope resolution 	Left-to-right
        ScopePrecedence = 1, 
        /*
        ++   -- 	Suffix/postfix increment and decrement
        type() type{} 	Function-style type cast
        () 	Function call
        [] 	Array subscripting
        . 	Element selection by reference
        -> 	Element selection through pointer
        */
        SuffixPrecedence = 2,
        /*
        ++   -- 	Prefix increment and decrement 	Right-to-left
        +   - 	Unary plus and minus
        !   ~ 	Logical NOT and bitwise NOT
        (type) 	C-style type cast
        * 	Indirection (dereference)
        & 	Address-of
        sizeof 	Size-of[note 1]
        new, new[] 	Dynamic memory allocation
        delete, delete[] 	Dynamic memory deallocation
        */
        PrefixPrecedence = 3,
        // .*   ->* 	Pointer to member 	Left-to-right
        PointerToMemberPrecedence = 4,
        // *   /   % 	Multiplication, division, and remainder
        MultiplicationPrecedence = 5,
        // 	+   - 	Addition and subtraction
        AdditionPrecedence = 6,
        // 	<<   >> 	Bitwise left shift and right shift
        BitwiseShiftPrecedence = 7,
        /*
        <   <= 	For relational operators < and <= respectively
        >   >= 	For relational operators > and >= respectively
        */
        RelationalOperatorPrecedence = 8,
        // ==   != 	For relational = and != respectively
        RelationalEqualityPrecedence = 9,
        // & 	Bitwise AND
        BitwiseAndPrecedence = 10,
        // ^ 	Bitwise XOR (exclusive or)
        BitwiseXorPrecedence = 11,
        // | 	Bitwise OR (inclusive or)
        BitwiseOrPrecedence = 12,
        // && 	Logical AND
        LogicalAndPrecedence = 13,
        // || 	Logical OR
        LogicalOrPrecedence = 14,
        /*
        ?: 	Ternary conditional[note 2] 	Right-to-left
        = 	Direct assignment (provided by default for C++ classes)
        +=   -= 	Assignment by sum and difference
        *=   /=   %= 	Assignment by product, quotient, and remainder
        <<=   >>= 	Assignment by bitwise left shift and right shift
        &=   ^=   |= 	Assignment by bitwise AND, XOR, and OR
        */
        AssignmentConditionPrecedence = 15,
        // throw 	Throw operator (for exceptions)
        ThrowPrecedence = 16,
        //, 	Comma 	Left-to-right
        CommaPrecedence = 17,
    };
}}}