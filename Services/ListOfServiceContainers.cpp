//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ListOfServiceContainers.h"
namespace DNVS {namespace MoFa {namespace Services {

    ListOfServiceContainers::ListOfServiceContainers(const std::list<std::shared_ptr<IServiceContainer>>& containers)
        :   m_containers(containers)
    {

    }

    bool ListOfServiceContainers::RegisterService(const std::type_info& index, std::shared_ptr<IServiceHolder> service, Lifetime lifetime)
    {
        for(auto it = m_containers.begin(); it != m_containers.end(); ++it)
        {
            auto container = *it;
            if(container->RegisterService(index, service, lifetime))
                return true;
        }
        return false;
    }

    std::shared_ptr<IServiceHolder> ListOfServiceContainers::LookupService(const std::type_info& index) const 
    {
        for(auto it = m_containers.begin(); it != m_containers.end(); ++it)
        {
            auto container = *it;
            auto service = container->LookupService(index);
            if(service)
                return service;
        }
        return nullptr;
    }

    bool ListOfServiceContainers::UnregisterService(const std::type_info& index)
    {
        for(auto it = m_containers.begin(); it != m_containers.end(); ++it)
        {
            auto container = *it;
            if(container->UnregisterService(index))
                return true;
        }
        return false;
    }

    void ListOfServiceContainers::ClearServices()
    {
        for(auto it = m_containers.begin(); it != m_containers.end(); ++it)
        {
            auto container = *it;
            container->ClearServices();
        }
    }

}}}