#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/StorageBase.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {

    //Movable smart pointers are smart pointers that can be moved, but not copied.
    //This includes std::unique_ptr

    template<class SmartPointerT>
    struct MovableSmartPointerStorage : public StorageBase {
    public:
        typedef MovableSmartPointerStorage BaseClass;
        MovableSmartPointerStorage(SmartPointerT&& smartPointer)
            :   m_smartPointer(std::move(smartPointer))
        {
            m_data = static_cast<void*>(m_smartPointer.get());
        }
        static bool IsConvertible(Variants::StoragePointer storage) {
            return dynamic_cast<MovableSmartPointerStorage*>(storage.get()) != 0;
        }

        template<typename U>
        static SmartPointerT& RecreateSmartPointer(Variants::StoragePointer storage, U* pointer)
        {
            auto ptr = dynamic_cast<MovableSmartPointerStorage*>(storage.get());
            if(!ptr)
                throw ConversionError(Types::TypeId<SmartPointerT>().GetTypeInfo(), Types::TypeId<U>().GetTypeInfo());
            return ptr->m_smartPointer;
        }
    private:
        SmartPointerT m_smartPointer;
    };
    
}}}}