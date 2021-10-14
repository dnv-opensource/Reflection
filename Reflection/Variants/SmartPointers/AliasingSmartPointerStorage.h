#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/StorageBase.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {
    //Aliasing smart pointers are smart pointers that can be reconstructed from a typeless smart pointer + a pointer.
    //This includes std::shared_ptr and Utility::SmartPointer.
    //Example
    //std::shared_ptr<Beam> a;
    //will be wrapped as 
    //std::shared_ptr<void> a + typeid(Beam)
    //If you later try to convert this into std::shared_ptr<StructureConcept> from a StructureConcept pointer, it will
    //use the aliasing constructor of the smart pointer:
    //std::shared_ptr<void> typelessPointer;
    //StructureConcept* concept;
    //std::shared_ptr<StructureConcept>(typelessPointer, concept);
    template<template<class> class SmartPointerT>
    struct AliasingSmartPointerStorage : public StorageBase {
    public:
        typedef AliasingSmartPointerStorage BaseClass;
        ///Construct a "stack" storage for the shared pointer. Store only the counter to the shared pointer.
        template<typename T>
        AliasingSmartPointerStorage(const SmartPointerT<T>& ptr) : m_storage(ptr,const_cast<void*>(static_cast<const void*>(get_pointer(ptr)))) {
            m_data=m_storage.get();
        }
        static bool IsConvertible(Variants::StoragePointer storage) {
            return dynamic_cast<AliasingSmartPointerStorage*>(storage.get())!=0;
        }
        ///Given a pointer, create a new shared_ptr with the same counter as m_storage.
        template<typename U>
        static SmartPointerT<U> RecreateSmartPointer(Variants::StoragePointer storage,U* pointer)
        {
            AliasingSmartPointerStorage* sharedPtrStorage=dynamic_cast<AliasingSmartPointerStorage*>(storage.get());
            return SmartPointerT<U>(sharedPtrStorage->m_storage,pointer);
        }
        virtual void* GetData() const { 
            return get_pointer(m_storage);
        }
    private:
        SmartPointerT<void> m_storage;
    };

    template<typename T>
    struct AliasingSmartPointerSelector;

    template<typename T, template<class> class SmartPointerT>
    struct AliasingSmartPointerSelector<SmartPointerT<T> > : AliasingSmartPointerStorage<SmartPointerT>
    {
        template<typename U>
        struct Rebind {
            typedef SmartPointerT<U> Type;
        };
        AliasingSmartPointerSelector(const SmartPointerT<T>& ptr) : BaseClass(ptr) {}
    };

}}}}