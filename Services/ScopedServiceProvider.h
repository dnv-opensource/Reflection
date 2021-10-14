#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ServiceProvider.h"

namespace DNVS {namespace MoFa {namespace Services {
    ///A ScopedServiceProvider automatically calls SetAsInstance(); on construction. Otherwise it is identical with ServiceProvider.
    ///It has proven problematic to call SetAsInstance() in the constructor. Especially when constructed early in the program flow. Usage is not recommended.
    class SERVICES_IMPORT_EXPORT ScopedServiceProvider : public ServiceProvider
    {
    public:
        ScopedServiceProvider(IServiceContainer::Lifetime lifetime = IServiceContainer::Session);
    };
}}}