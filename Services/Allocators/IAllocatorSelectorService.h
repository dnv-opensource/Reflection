#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAllocatorSelector.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {
    class IAllocatorSelectorService
    {
    public:        
        virtual ~IAllocatorSelectorService() {}
        virtual TypeUtilities::IntrusivePointer<IAllocatorSelector> GetAllocatorSelector() = 0;
    };
}}}}

