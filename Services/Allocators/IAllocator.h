#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <typeinfo>
#include "TypeUtilities/IntrusiveClass.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {

    class IAllocator : public TypeUtilities::IntrusiveClass<IAllocator>
    {
    public:
        virtual ~IAllocator() {}
        virtual void* Allocate(size_t count, size_t objectSize, const type_info& info) = 0;
        virtual void Deallocate(void* ptr, size_t count) = 0;
    };

    typedef TypeUtilities::IntrusivePointer<IAllocator> AllocatorPointer;
}}}}
