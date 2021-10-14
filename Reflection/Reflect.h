#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>
#include "DoReflect.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"

namespace DNVS { namespace MoFa { namespace Reflection { 
    /**
    Use this function when you want to register reflection for a given type.
    The syntax is for regular classes:    
    Reflect<FloodingMemOpt>(typeLibrary);

    For some template classes where the type is part of the input, you need to add other arguments, such as the name:
    Reflect<EnumOrValue<MyEnum, Manual, Length>>(typeLibrary,"MyEnumOrLength");
    
    For classes where you have overloaded the allocator, you need to pass the allocator:
    Services::Allocators::ForwardingAllocatorSelector allocator;
    Reflect<ParametricArc2D>(typeLibrary,allocator);

    Using this function requires that you implement DoReflect for your class. See documentation for DoReflect for more details.
    */
    template<typename YourTypeGoesHere, typename... Args>
    void Reflect(const TypeLibraries::TypeLibraryPointer& typeLibrary, Args&&... args)
    {
        if (!typeLibrary->IsTypeReflected(typeid(YourTypeGoesHere)))
        {
            typeLibrary->SetTypeReflected(typeid(YourTypeGoesHere));
            DoReflect(typeLibrary, std::forward<Args>(args)..., (YourTypeGoesHere**)nullptr);            
        }
    }
}}}
