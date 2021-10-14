//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ScopedServiceProvider.h"
namespace DNVS {namespace MoFa {namespace Services {

    ScopedServiceProvider::ScopedServiceProvider(IServiceContainer::Lifetime lifetime) 
        :   ServiceProvider(lifetime)
    {
        SetAsInstance();
    }

}}}