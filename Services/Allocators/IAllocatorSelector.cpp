//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PersistenceControl.h"
#include "..\ServiceProvider.h"
#include "IAllocatorSelectorService.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {

    TypeUtilities::IntrusivePointer<IAllocatorSelector> GetAllocatorSelector()
    {
        auto allocatorSelectorService = ServiceProvider::Instance().TryGetService<IAllocatorSelectorService>();
        if (allocatorSelectorService)
            return allocatorSelectorService->GetAllocatorSelector();
        else
            return nullptr;
    }

}}}}


