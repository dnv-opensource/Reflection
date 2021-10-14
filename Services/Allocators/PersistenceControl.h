#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAllocatorSelector.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {
    //This class ensures that allocations made with the IAllocatorSelector are done in persistent memory.
    class SERVICES_IMPORT_EXPORT PersistenceControl
    {
    public:
        PersistenceControl(bool isPersistent);
        PersistenceControl(TypeUtilities::IntrusivePointer<IAllocatorSelector> allocator, bool isPersistent);
        ~PersistenceControl();
    private:
        TypeUtilities::IntrusivePointer<IAllocatorSelector> m_allocator;
        bool m_oldIsPersistent;
    };
}}}}
