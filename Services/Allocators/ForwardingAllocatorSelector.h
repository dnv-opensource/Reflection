#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "ForwardingAllocator.h"
#include "IAllocatorSelector.h"
#include "TypeUtilities/PointerHolderConstructor.h"
#include <memory>
#include "GenericDeleter.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {

    class ForwardingAllocatorSelector {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef void* pointer;
        typedef const void* const_pointer;
        typedef void value_type;
        typedef ForwardingAllocatorSelector alloc_type;

        template <class Other> struct rebind {
            typedef ForwardingAllocator<Other> other;
        };
        ForwardingAllocatorSelector() {}
        ForwardingAllocatorSelector(TypeUtilities::IntrusivePointer<IAllocatorSelector> selector)
            :   m_selector(selector)
        {}
        operator AllocatorPointer() const {
            if(m_selector)
                return m_selector->GetCurrentAllocator();
            else
                return nullptr;
        }
    private:
        TypeUtilities::IntrusivePointer<IAllocatorSelector> m_selector;
    };

}}}}
namespace DNVS {namespace MoFa {namespace TypeUtilities {

    template<typename T>
    class PointerHolderConstructor<std::shared_ptr<T>, Services::Allocators::ForwardingAllocatorSelector>
    {
    public:
        template<typename U>
        static std::shared_ptr<T> WrapPointer(U pointer, const Services::Allocators::ForwardingAllocatorSelector& alloc) { return std::shared_ptr<T>(pointer, Services::Allocators::GenericDeleter(), alloc); }
    };
}}}