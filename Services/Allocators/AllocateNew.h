#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {
    template<typename T,typename AllocatorT, typename... Args>
    T* AllocateNew(const AllocatorT& allocator, Args&&... args)
    {
        typename AllocatorT::template rebind<T>::other actualAllocator(allocator);

        T* pointer = actualAllocator.allocate(1);

        try {
            actualAllocator.construct(pointer, std::forward<Args>(args)...);
        }
        catch (...)
        {
            actualAllocator.deallocate(pointer, 1);
            throw;
        }
        return pointer;
    }
}}}}