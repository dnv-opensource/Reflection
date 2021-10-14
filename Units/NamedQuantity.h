#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include <string>

namespace DNVS {namespace MoFa {namespace Units {
    template<typename DimensionT>
    class NamedQuantity : public QuantityBase<DimensionT>
    {
    public:
        NamedQuantity(const NamedQuantity& other) 
            :   BaseClass(other) 
            ,   m_name(other.m_name)
        {}
        explicit NamedQuantity(double value, const std::string& name) 
            :   BaseClass(value) 
            ,   m_name(name)
        {}
        NamedQuantity& operator=(const NamedQuantity& other) 
        {   
            BaseClass::operator=(other); 
            m_name = other.m_name;
            return *this; 
        }
        const std::string GetName() const {return m_name; }
    private:
        std::string m_name;
    };

}}}
