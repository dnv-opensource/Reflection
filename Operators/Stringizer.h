#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Operators\Tags.h>
#include <string>

namespace DNVS {namespace MoFa {namespace Operators {
    //For any given operator tag, you can evaluate the expression using an Stringizer. 
    //Stringizer<Tags::Plus>::Invoke(5,9.3); will return the result of 5+9.3.
    //This is useful when writing a generic class to handle all binary operators in a consistent manner.

    template<typename Tag>
    struct Stringizer;

    template<>
    struct Stringizer<Tags::UnaryPlus>
    {
        static std::string Name() {
            return "UnaryPlus";
        }
        static std::string Stringize(const std::string& a0) {
            return "+" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::Negate>
    {
        static std::string Name() {
            return "Negate";
        }
        static std::string Stringize(const std::string& a0) {
            return "-" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::Dereference>
    {
        static std::string Name() {
            return "Dereference";
        }
        static std::string Stringize(const std::string& a0) {
            return "*" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::Complement>
    {
        static std::string Name() {
            return "Complement";
        }
        static std::string Stringize(const std::string& a0) {
            return "~" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::AddressOf>
    {
        static std::string Name() {
            return "AddressOf";
        }
        static std::string Stringize(const std::string& a0) {
            return "&" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::LogicalNot>
    {
        static std::string Name() {
            return "LogicalNot";
        }
        static std::string Stringize(const std::string& a0) {
            return "!" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::PreInc>
    {
        static std::string Name() {
            return "PreInc";
        }
        static std::string Stringize(const std::string& a0) {
            return "++" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::PreDec>
    {
        static std::string Name() {
            return "PreDec";
        }
        static std::string Stringize(const std::string& a0) {
            return "--" + a0;
        }
    };

    template<>
    struct Stringizer<Tags::PostInc>
    {
        static std::string Name() {
            return "PostInc";
        }
        static std::string Stringize(const std::string& a0) {
            return a0+"++";
        }
    };

    template<>
    struct Stringizer<Tags::PostDec>
    {
        static std::string Name() {
            return "PostDec";
        }
        static std::string Stringize(const std::string& a0) {
            return a0+"--";
        }
    };

    template<>
    struct Stringizer<Tags::ShiftLeft>
    {
        static std::string Name() {
            return "ShiftLeft";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" << " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::ShiftRight>
    {
        static std::string Name() {
            return "ShiftRight";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" >> " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Multiplies>
    {
        static std::string Name() {
            return "Multiplies";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" * " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Divides>
    {
        static std::string Name() {
            return "Divides";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" / " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Modulus>
    {
        static std::string Name() {
            return "Modulus";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" % " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Plus>
    {
        static std::string Name() {
            return "Plus";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" + " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Minus>
    {
        static std::string Name() {
            return "Minus";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" - " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Less>
    {
        static std::string Name() {
            return "Less";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" < " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Greater>
    {
        static std::string Name() {
            return "Greater";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" > " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::LessEqual>
    {
        static std::string Name() {
            return "LessEqual";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" <= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::GreaterEqual>
    {
        static std::string Name() {
            return "GreaterEqual";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" >= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::EqualTo>
    {
        static std::string Name() {
            return "EqualTo";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" == " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::NotEqualTo>
    {
        static std::string Name() {
            return "NotEqualTo";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" != " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::LogicalOr>
    {
        static std::string Name() {
            return "LogicalOr";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" || " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::LogicalAnd>
    {
        static std::string Name() {
            return "LogicalAnd";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" && " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::BitwiseAnd>
    {
        static std::string Name() {
            return "BitwiseAnd";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" & " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::BitwiseOr>
    {
        static std::string Name() {
            return "BitwiseOr";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" | " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::BitwiseXor>
    {
        static std::string Name() {
            return "BitwiseXor";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" ^ " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Assign>
    {
        static std::string Name() {
            return "Assign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" = " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::ShiftLeftAssign>
    {
        static std::string Name() {
            return "ShiftLeftAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" <<= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::ShiftRightAssign>
    {
        static std::string Name() {
            return "ShiftRightAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" >>= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::MultipliesAssign>
    {
        static std::string Name() {
            return "MultipliesAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" *= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::DividesAssign>
    {
        static std::string Name() {
            return "DividesAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" /= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::ModulusAssign>
    {
        static std::string Name() {
            return "ModulusAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" %= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::PlusAssign>
    {
        static std::string Name() {
            return "PlusAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" += " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::MinusAssign>
    {
        static std::string Name() {
            return "MinusAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" -= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::BitwiseAndAssign>
    {
        static std::string Name() {
            return "BitwiseAndAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" &= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::BitwiseOrAssign>
    {
        static std::string Name() {
            return "BitwiseOrAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" |= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::BitwiseXorAssign>
    {
        static std::string Name() {
            return "BitwiseXorAssign";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+" ^= " + a1;
        }
    };

    template<>
    struct Stringizer<Tags::Subscript>
    {
        static std::string Name() {
            return "Subscript";
        }
        static std::string Stringize(const std::string& a0,const std::string& a1) {
            return a0+"[" + a1 + "]";
        }
    };
}}}
