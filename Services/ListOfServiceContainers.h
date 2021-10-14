#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IServiceContainer.h"

#include <list>
namespace DNVS {namespace MoFa {namespace Services {
    class SERVICES_IMPORT_EXPORT ListOfServiceContainers : public IServiceContainer
    {
    public:
        ListOfServiceContainers(const std::list<std::shared_ptr<IServiceContainer>>& containers);
        virtual bool RegisterService(const std::type_info& index, std::shared_ptr<IServiceHolder> service, Lifetime lifetime) override;
        virtual bool UnregisterService(const std::type_info& index) override;
        virtual void ClearServices() override;
        virtual std::shared_ptr<IServiceHolder> LookupService(const std::type_info& index) const override;
    private:
        std::list<std::shared_ptr<IServiceContainer>> m_containers;
    };
}}}

