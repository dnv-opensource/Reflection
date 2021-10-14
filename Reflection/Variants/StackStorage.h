#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Variants/StorageBase.h"
#include <type_traits>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {

    template<typename T>
    struct ConstReferenceToVoidPointerCast
    {
        ConstReferenceToVoidPointerCast(const T& value_)
            : value(value_)
        {}
        operator void*() const { return const_cast<T*>(&value); }
        const T& value;
    };

    template<typename T>
    class StackStorage : public StorageBase
    {
    public:
        StackStorage(const T& value) 
            :   m_value(value) 
        {            
            m_data = ConstReferenceToVoidPointerCast<std::remove_const_t<T>>(m_value);
        }
        virtual ~StackStorage() {}
    private:
        typename std::remove_const<T>::type m_value;
    };

}}}}
