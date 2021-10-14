#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    class IVisitor {
    public:
        virtual ~IVisitor() {}
    };

    template<typename T>
    class TypedVisitor : virtual public IVisitor
    {
    public:
        virtual ~TypedVisitor() {}
        virtual void Visit(const T&) = 0;
    };

    template<typename... T>
    class Visitors;

    template<typename T, typename... ArgsT>
    class Visitors<T, ArgsT...> : public TypedVisitor<T>, public Visitors<ArgsT...>
    {};

    template<>
    class Visitors<>
    {};

}}}