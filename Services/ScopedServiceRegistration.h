#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ServiceProvider.h"

namespace DNVS {namespace MoFa {namespace Services {
    template<typename T>
    class ScopedServiceRegistration
    {
    public:        
        ScopedServiceRegistration(std::shared_ptr<T> service, ServiceProvider& serviceProvider)
            : m_serviceProvider(serviceProvider)
        {
            m_oldService = m_serviceProvider.TryGetService<T>();
            if (service)
                m_serviceProvider.RegisterService(service);
            else
                m_serviceProvider.UnregisterService<T>();
        }

        ScopedServiceRegistration(std::shared_ptr<T> service)
            :ScopedServiceRegistration(service, DNVS::MoFa::Services::ServiceProvider::Instance())
        {
        }
        ~ScopedServiceRegistration()
        {
            if (m_oldService)
                m_serviceProvider.RegisterService(m_oldService);
            else
                m_serviceProvider.UnregisterService<T>();
        }
    private:
        std::shared_ptr<T> m_oldService;
        ServiceProvider& m_serviceProvider;
    };
    template<typename T, typename U, typename... Args>
    std::unique_ptr<ScopedServiceRegistration<T>> ConditionallyCreateScopedServiceRegistration(Args&&... args)
    {
        if (DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<T>())
            return nullptr;
        else
            return std::unique_ptr<ScopedServiceRegistration<T>>(new ScopedServiceRegistration<T>(std::make_shared<U>(std::forward<Args>(args)...)));
    }
}}}