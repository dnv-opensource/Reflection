#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Operators\Tags.h>

namespace DNVS {namespace MoFa {namespace Operators {
    //Define results of all operator expressions. This is used to find the result type of e.g. double+int or std::string + const char*
    template<typename TagAndArguments>
    struct ResultOf;
    namespace Detail {
        template<typename T>
        T Make();
    }

    template<typename A0>
    struct ResultOf<Tags::UnaryPlus(A0)>
    {
        typedef decltype(+Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::Negate(A0)>
    {
        typedef decltype(-Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::Dereference(A0)>
    {
        typedef decltype(*Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::Complement(A0)>
    {
        typedef decltype(~Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::AddressOf(A0)>
    {
        typedef decltype(&Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::LogicalNot(A0)>
    {
        typedef decltype(!Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::PreInc(A0)>
    {
        typedef decltype(++Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::PreDec(A0)>
    {
        typedef decltype(--Detail::Make<A0>()) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::PostInc(A0)>
    {
        typedef decltype(Detail::Make<A0>()++) result_type;
    };

    template<typename A0>
    struct ResultOf<Tags::PostDec(A0)>
    {
        typedef decltype(Detail::Make<A0>()--) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::ShiftLeft(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()<<Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::ShiftRight(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()>>Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Multiplies(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()*Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Divides(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()/Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Modulus(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()%Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Plus(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()+Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Minus(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()-Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Less(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()<Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Greater(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()>Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::LessEqual(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()<=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::GreaterEqual(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()>=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::EqualTo(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()==Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::NotEqualTo(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()!=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::LogicalOr(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()||Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::LogicalAnd(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()&&Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::BitwiseAnd(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()&Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::BitwiseOr(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()|Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::BitwiseXor(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()^Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Assign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::ShiftLeftAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()<<=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::ShiftRightAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()>>=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::MultipliesAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()*=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::DividesAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()/=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::ModulusAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()%=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::PlusAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()+=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::MinusAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()-=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::BitwiseAndAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()&=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::BitwiseOrAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()|=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::BitwiseXorAssign(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()^=Detail::Make<A1>()) result_type;
    };

    template<typename A0,typename A1>
    struct ResultOf<Tags::Subscript(A0,A1)>
    {
        typedef decltype(Detail::Make<A0>()[Detail::Make<A1>()]) result_type;
    };
}}}
