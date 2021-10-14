#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
#include "Reflection/AutoReflect.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "../Attributes/TupleAttribute.h"
#include "../Variants/VariantService.h"
#include "GenericTupleReflect.h"
#define REFLECTOR_LIST_INCLUDED

namespace DNVS { namespace MoFa { namespace Reflection {
    template<typename... Args>
    struct AutoReflectImplemented<std::tuple<Args...>, void>
    {
        static const bool value = true;
    };
}}}
namespace std 
{
    template<typename... Args>
    void DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, tuple<Args...>**)
    {        
		using namespace DNVS::MoFa::Reflection;
        using Tuple = tuple<Args...>;
		using NonConstTuple = std::tuple<std::remove_const_t<Args>...>;
		AutoReflectVariadic<std::remove_const_t<Args>...>::Reflect(typeLibrary);
		GenericTupleReflector<Tuple, NonConstTuple>::Reflect(typeLibrary);
    };
}