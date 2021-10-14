#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Domain.h"

namespace DNVS {namespace MoFa {namespace Operators {
    //You need to overload DomainBinaryOperatorResult for your domain
    template<typename Domain,typename Tag,typename Lhs,typename Rhs>
    class DomainBinaryOperatorResult
    {
        typedef void type;
    };


    template<typename Tag,typename Lhs,typename Rhs>
    class BinaryOperatorResult : public DomainBinaryOperatorResult<typename DeduceBinaryDomain<Lhs,Rhs>::type,Tag,Lhs,Rhs> 
    {};
}}}