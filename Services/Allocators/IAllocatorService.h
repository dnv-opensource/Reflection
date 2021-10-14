#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "..\Config.h"
#include "IAllocator.h"

namespace DNVS { namespace MoFa { namespace Services { namespace Allocators {
    class SERVICES_IMPORT_EXPORT IAllocatorService 
    {
    public:
        virtual ~IAllocatorService() {}
        virtual AllocatorPointer GetAllocatorFromPointer(void* ptr) = 0;
        static AllocatorPointer TryGetAllocatorFromPointer(void* ptr);
    };
}}}}