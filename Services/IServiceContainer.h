#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Config.h"
#include "ServiceHolder.h"
#include <typeindex>
#include <memory>

namespace DNVS {namespace MoFa {namespace Services {
    ///IServiceContainer is responsible for storing a collection of services.
    class SERVICES_IMPORT_EXPORT IServiceContainer
    {
    public:
        virtual ~IServiceContainer() {}
        enum Lifetime 
        {
            Session,
            Permanent,
        };        
        virtual bool RegisterService(const std::type_info& index, std::shared_ptr<IServiceHolder> service, Lifetime lifetime) = 0;
        virtual bool UnregisterService(const std::type_info& index) = 0;
        virtual void ClearServices() = 0;
        virtual std::shared_ptr<IServiceHolder> LookupService(const std::type_info& index) const = 0;
    };
}}}

