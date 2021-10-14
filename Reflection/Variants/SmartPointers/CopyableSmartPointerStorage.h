#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/StorageBase.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {


    //Copyable smart pointers are smart pointers that can be reconstructed from a pointer, or not at all.
    //This includes std::intrusive_ptr    

    class ICopyableSmartPointer
    {
    public:
        virtual ~ICopyableSmartPointer() {}
    };

    template<typename U>
    class CopyableSmartPointer : public ICopyableSmartPointer 
    {
    public:
        CopyableSmartPointer(const U& smartPointer) : m_smartPointer(smartPointer) {}
    private:
        U m_smartPointer;    
    };

    template<template<class> class SmartPointerT>
    struct CopyableSmartPointerStorage : public StorageBase {
    public:
        typedef CopyableSmartPointerStorage BaseClass;
        template<typename T>
        CopyableSmartPointerStorage(SmartPointerT<T> smartPointer)
        {
            m_storage = new CopyableSmartPointer<SmartPointerT<T> >(smartPointer);
            m_data = static_cast<void*>(smartPointer.get());
        }
        virtual ~CopyableSmartPointerStorage() {delete m_storage; }
        static bool IsConvertible(Variants::StoragePointer storage, bool strict = false) 
        {
            if (!strict && storage == nullptr)
                return true;
            return dynamic_cast<CopyableSmartPointerStorage*>(storage.get()) != 0;
        }

        //Assumes this is an intrusive pointer. Otherwise we cannot unwrap the pointer.
        template<typename U>
        static U* RecreateSmartPointer(Variants::StoragePointer storage, U* pointer)
        {
            return pointer;
        }
    private:
        ICopyableSmartPointer* m_storage;
    };

    template<template<class, class> class SmartPointerT, typename D>
    struct CopyableSmartPointerStorage2 : public StorageBase {
    public:
        typedef CopyableSmartPointerStorage2 BaseClass;
        template<typename T>
        CopyableSmartPointerStorage2(SmartPointerT<T, D> smartPointer)
        {
            m_storage = new CopyableSmartPointer<SmartPointerT<T, D> >(smartPointer);
            m_data = static_cast<void*>(smartPointer.get());
        }
        virtual ~CopyableSmartPointerStorage2() {delete m_storage; }
        static bool IsConvertible(Variants::StoragePointer storage, bool strict = false) 
        {
            if (!strict && storage == nullptr)
                return true;
            return dynamic_cast<CopyableSmartPointerStorage2*>(storage.get()) != 0;
        }

        //Assumes this is an intrusive pointer. Otherwise we cannot unwrap the pointer.
        template<typename U>
        static U* RecreateSmartPointer(Variants::StoragePointer storage, U* pointer)
        {
            return pointer;
        }
    private:
        ICopyableSmartPointer* m_storage;
    };

    template<typename T>
    struct CopyableSmartPointerSelector;

    template<typename T, template<class> class SmartPointerT>
    struct CopyableSmartPointerSelector<SmartPointerT<T> > : CopyableSmartPointerStorage<SmartPointerT>
    {
        template<typename U>
        struct Rebind {
            typedef SmartPointerT<U> Type;
        };
        CopyableSmartPointerSelector(const SmartPointerT<T>& ptr) : BaseClass(ptr) {}
    };

    template<typename T, typename D, template<class, class> class SmartPointerT>
    struct CopyableSmartPointerSelector<SmartPointerT<T, D> > : CopyableSmartPointerStorage2<SmartPointerT, D>
    {
        template<typename U>
        struct Rebind {
            typedef SmartPointerT<U, D> Type;
        };
        CopyableSmartPointerSelector(const SmartPointerT<T, D>& ptr) : BaseClass(ptr) {}
    };


}}}}