#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Services/Allocators/ForwardingAllocatorSelector.h"
#include "Plugins/ISubsystem.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Plugins {
    class IReflectionSubsystem : public DNVS::MoFa::Plugins::ISubsystem
    {
    public:
        virtual void RegisterReflection(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Services::Allocators::ForwardingAllocatorSelector& allocator) = 0;
    };
}}}}