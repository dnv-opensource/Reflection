//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "PersistenceControl.h"
#include "..\ServiceProvider.h"
#include "IAllocatorSelectorService.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {

    PersistenceControl::PersistenceControl(bool isPersistent)
        : PersistenceControl(GetAllocatorSelector(), isPersistent)
    {
    }

    PersistenceControl::PersistenceControl(TypeUtilities::IntrusivePointer<IAllocatorSelector> allocator, bool isPersistent) 
        : m_allocator(allocator)
    {
        if (m_allocator)
        {
            m_oldIsPersistent = m_allocator->GetPersistentMode();
            m_allocator->SetPersistentMode(isPersistent);
        }
    }

    PersistenceControl::~PersistenceControl()
    {
        if(m_allocator)
            m_allocator->SetPersistentMode(m_oldIsPersistent);
    }

}}}}




