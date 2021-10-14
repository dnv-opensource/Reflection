#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Config.h"
#include "IServiceContainer.h"
#include "ServiceHolder.h"
#include <string>

namespace DNVS {namespace MoFa {namespace Services {
    ///ServiceProvider is a global singleton that holds a collection of services. 
    ///In your application, you can construct multiple service providers with different lifetimes.
    ///We have two lifetimes: 
    /// - Session - From you open a database until you close it
    /// - Permanent - From you open the application until you close it
    ///Internally, the collection of services will be merged. When you register a new service, you have to specify a lifetime in order to register the service with the correct provider.
    ///The default is Session.
    class SERVICES_IMPORT_EXPORT ServiceProvider
    {
    public:
        ServiceProvider(IServiceContainer::Lifetime lifetime);
        ~ServiceProvider();
        template<typename Interface, typename ConcreteService>
        typename std::enable_if<std::is_base_of<Interface, ConcreteService>::value && !std::is_same<Interface, ConcreteService>::value, void>::type RegisterService(std::shared_ptr<ConcreteService> service, IServiceContainer::Lifetime lifetime = IServiceContainer::Session)
        {
            if(!m_serviceContainer->RegisterService(typeid(Interface), std::make_shared<ServiceHolder<Interface>>(service), lifetime))
                throw std::runtime_error("Unable to register " + std::string(typeid(ConcreteService).name()) + ". Unsupported service lifetime.");
        }
        template<typename Interface>
        void RegisterService(std::shared_ptr<Interface> service, IServiceContainer::Lifetime lifetime = IServiceContainer::Session)
        {
            if(!m_serviceContainer->RegisterService(typeid(Interface), std::make_shared<ServiceHolder<Interface>>(service), lifetime))
                throw std::runtime_error("Unable to register " + std::string(typeid(Interface).name()) + ". Unsupported service lifetime.");
        }
        template<typename Interface>
        void UnregisterService() 
        {
            m_serviceContainer->UnregisterService(typeid(Interface));
        }
        template<typename Interface>
        std::shared_ptr<Interface> GetService() 
        {
            auto service = TryGetService<Interface>();
            if(service)
                return service;
            throw std::runtime_error(std::string(typeid(Interface).name()) + " not registered to the ServiceProvider");
        }
        template<typename Interface>
        std::shared_ptr<Interface> TryGetService() const
        {
            auto serviceHolder = m_serviceContainer->LookupService(typeid(Interface));
            if(serviceHolder)
            {
                std::shared_ptr<ServiceHolder<Interface> > service = std::static_pointer_cast<ServiceHolder<Interface> >(serviceHolder);
                return service->GetService();
            }
            return nullptr;
        }
        void ClearAllServices();
        void SetAsInstance();
        static ServiceProvider& Instance();
    private:
        std::shared_ptr<IServiceContainer> GetServiceContainer();
        std::shared_ptr<IServiceContainer> m_serviceContainer;
        ServiceProvider* m_oldProvider;
        static ServiceProvider* s_instance;
    };
}}}

