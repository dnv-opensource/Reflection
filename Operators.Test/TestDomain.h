#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Operators\OperatorBase.h"
#include "Operators\BinaryOperatorResult.h"
#include "Operators\UnaryOperatorResult.h"

struct MyDomain {
};

class Base : public DNVS::MoFa::Operators::OperatorNamespace::OperatorBase
{
public:
    typedef MyDomain Domain;
};

template<typename Tag,typename Lhs,typename Rhs>
class BinaryResult : public DNVS::MoFa::Operators::OperatorNamespace::OperatorBase{
public:
    BinaryResult(Lhs lhs,Rhs rhs) {}
    typedef MyDomain Domain;
};

template<typename Tag,typename Subject>
class UnaryResult : public DNVS::MoFa::Operators::OperatorNamespace::OperatorBase{
public:
    UnaryResult(Subject subject) {}
    typedef MyDomain Domain;
};

namespace DNVS {namespace MoFa {namespace Operators {
    template<typename Tag,typename Lhs,typename Rhs>
    class DomainBinaryOperatorResult<MyDomain,Tag,Lhs,Rhs>
    {
    public:
        typedef BinaryResult<Tag,Lhs,Rhs> type;
        static BinaryResult<Tag,Lhs,Rhs> BuildExpression(const Lhs& lhs,const Rhs& rhs) {
            return BinaryResult<Tag,Lhs,Rhs>(lhs,rhs);
        }
    };
    template<typename Tag,typename Subject>
    class DomainUnaryOperatorResult<MyDomain,Tag,Subject>
    {
    public:
        typedef UnaryResult<Tag,Subject> type;
        static UnaryResult<Tag,Subject> BuildExpression(const Subject& subject) {
            return UnaryResult<Tag,Subject>(subject);
        }
    };
}}}
