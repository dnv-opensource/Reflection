//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAllocatorService.h"
#include "..\ServiceProvider.h"
namespace DNVS { namespace MoFa { namespace Services { namespace Allocators {

    AllocatorPointer IAllocatorService::TryGetAllocatorFromPointer(void* ptr)
    {
        auto service = ServiceProvider::Instance().TryGetService<IAllocatorService>();
        if(service)
            return service->GetAllocatorFromPointer(ptr);
        else
            return nullptr;
    }

}}}}