#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAllocatorSelector.h"
#include "ForwardingAllocator.h"
#include <memory>
#include "PersistenceControl.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {

    class PersistentAllocator
    {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef void* pointer;
        typedef const void* const_pointer;
        typedef void value_type;
        typedef PersistentAllocator alloc_type;

        template <class Other> struct rebind {
            typedef ForwardingAllocator<Other> other;
        };
        PersistentAllocator() 
            : m_selector(GetAllocatorSelector())
        {}
        operator AllocatorPointer() const {
            PersistenceControl persistent(true);            
            if(m_selector)
                return m_selector->GetCurrentAllocator();
            else
                return nullptr;
        }
    private:
        TypeUtilities::IntrusivePointer<IAllocatorSelector> m_selector;
    };

}}}}