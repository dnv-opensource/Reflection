#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Services {
    ///IServiceHolder is responsible for storing a single service. 
    ///Since services don't have a common base class, we use IServiceHolder as base class, and ServiceHolder<T> as the class to represent the actual service, where T is the interface of the service in question.
    ///ServiceHolder<T> inherits from IServiceHolder
    class IServiceHolder 
    {
    public:
        virtual ~IServiceHolder() {}
    };
}}}
