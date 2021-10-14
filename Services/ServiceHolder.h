#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IServiceHolder.h"
#include <memory>

namespace DNVS {namespace MoFa {namespace Services {

    template<typename T>
    class ServiceHolder : public IServiceHolder
    {
    public:
        ServiceHolder(const std::shared_ptr<T>& service) : m_service(service) {}
        ServiceHolder(const ServiceHolder& other) : m_service(other.m_service) {}
        ServiceHolder& operator=(const ServiceHolder& other) 
        {
            m_service = other.m_service;
            return *this;
        }
        const std::shared_ptr<T>& GetService() const {return m_service; }
    private:
        std::shared_ptr<T> m_service;
    };
}}}
