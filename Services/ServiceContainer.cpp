//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ServiceContainer.h"
namespace DNVS {namespace MoFa {namespace Services {

    ServiceContainer::ServiceContainer(Lifetime lifetime) 
        :   m_lifetime(lifetime)
    {

    }

    bool ServiceContainer::RegisterService(const std::type_info& index, std::shared_ptr<IServiceHolder> service, Lifetime lifetime)
    {
        if(lifetime == m_lifetime)
        {
            m_services[index] = service;
            return true;
        }
        else
            return false;
    }

    std::shared_ptr<IServiceHolder> ServiceContainer::LookupService(const std::type_info& index) const 
    {
        auto it = m_services.find(index);
        if(it != m_services.end())
            return it->second;
        //This is much much cheaper than returning nullptr.
        return std::shared_ptr<IServiceHolder>();
    }

    bool ServiceContainer::UnregisterService(const std::type_info& index)
    {
        //Hold a pointer to the service.
        auto service = LookupService(index);
        //Remove the service from m_services.
        return (m_services.erase(index) > 0);
        //Run destructor of service.
    }

    void ServiceContainer::ClearServices()
    {
        m_services.clear();
    }

}}}