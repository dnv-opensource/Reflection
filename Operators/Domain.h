#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "OperatorBase.h"
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Operators {
    //In order to limit the use of our operators, we need to define the domain in which they are to be used.
    //First of all, we only allow the operators to be available if one of the input classes inherit from OperatorNamespace::OperatorBase
    //If we have two unrelated classes inheriting from OperatorNamespace::OperatorBase, we do not want them to interact.
    //If we have a primitive value and a class inheriting from OperatorNamespace::OperatorBase, they can interact.
    //These classes enforce this logic.
    //In your class, you need to inherit from OperatorNamespace::OperatorBase and typedef a Domain in this class.
    //This domain can have the form: struct MyDomain {};
    struct NoDomain;

    template<typename T,typename Enable=void>
    struct DeduceDomain {
        typedef NoDomain type;
    };

    template<typename T>
    struct DeduceDomain<T,std::enable_if_t<std::is_base_of_v<OperatorNamespace::OperatorBase,T>>>
    {
        typedef typename T::Domain type;
    };

    template<typename Lhs,typename Rhs>
    struct DeduceBinaryDomainImpl
    {
        typedef NoDomain type;
    };
    template<typename Rhs>
    struct DeduceBinaryDomainImpl<NoDomain,Rhs>
    {
        typedef Rhs type;
    };
    template<typename Lhs>
    struct DeduceBinaryDomainImpl<Lhs,NoDomain>
    {
        typedef Lhs type;
    };
    template<>
    struct DeduceBinaryDomainImpl<NoDomain,NoDomain>
    {
        typedef NoDomain type;
    };
    template<typename Subject>
    struct DeduceBinaryDomainImpl<Subject,Subject>
    {
        typedef Subject type;
    };

    template<typename Lhs,typename Rhs>
    struct DeduceBinaryDomain
    {
        typedef typename DeduceDomain<Lhs>::type LhsDomain;
        typedef typename DeduceDomain<Rhs>::type RhsDomain;
        typedef typename DeduceBinaryDomainImpl<LhsDomain,RhsDomain>::type type;
    };

    template<typename T>
    struct IsDomain : std::true_type {};

    template<>
    struct IsDomain<NoDomain> : std::false_type {};

    template<typename Lhs,typename Rhs>
    struct IsBinaryDomain : IsDomain<typename DeduceBinaryDomain<Lhs,Rhs>::type> {};

    template<typename Subject>
    struct IsUnaryDomain : IsDomain<typename DeduceDomain<Subject>::type> {};
}}}