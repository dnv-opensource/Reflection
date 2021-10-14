#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <typeinfo>
#include "TypeUtilities/IntrusiveClass.h"
#include "IAllocator.h"
#include "../Config.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {
    class IAllocatorSelector : public TypeUtilities::IntrusiveClass<IAllocatorSelector>
    {
    public:
        virtual ~IAllocatorSelector() {}
        virtual AllocatorPointer GetCurrentAllocator() = 0;
        virtual void SetPersistentMode(bool persistentMode) = 0;
        virtual bool GetPersistentMode() const = 0;
    };
    
    TypeUtilities::IntrusivePointer<IAllocatorSelector> SERVICES_IMPORT_EXPORT GetAllocatorSelector();
}}}}
