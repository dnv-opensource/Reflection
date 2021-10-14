//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ServiceProvider.h"
#include "ServiceContainer.h"
#include "ListOfServiceContainers.h"

namespace DNVS {namespace MoFa {namespace Services {
    ServiceProvider::ServiceProvider(IServiceContainer::Lifetime lifetime) 
        :   m_oldProvider(nullptr)
        ,   m_serviceContainer(new ServiceContainer(lifetime))
    {
    }

    ServiceProvider::~ServiceProvider() 
    {
        if(this == s_instance)
        {
            s_instance = m_oldProvider;
        }
    }

    ServiceProvider& ServiceProvider::Instance()
    {
        if(s_instance)
            return *s_instance;
        else
        {
            static ServiceProvider dummyProvider(IServiceContainer::Session);
            dummyProvider.ClearAllServices();
            return dummyProvider;
        }
    }

    void ServiceProvider::ClearAllServices()
    {
        m_serviceContainer->ClearServices();
    }

    void ServiceProvider::SetAsInstance()
    {
        if(s_instance == this)
            return;
        m_oldProvider = s_instance;
        s_instance = this;
        if(m_oldProvider)
        {
            std::list<std::shared_ptr<IServiceContainer>> containers;
            containers.push_back(m_oldProvider->GetServiceContainer());
            containers.push_back(GetServiceContainer());
            m_serviceContainer = std::make_shared<ListOfServiceContainers>(containers);
        }
    }
    
    std::shared_ptr<IServiceContainer> ServiceProvider::GetServiceContainer()
    {
        return m_serviceContainer;
    }

    ServiceProvider* ServiceProvider::s_instance = 0;
}}}

