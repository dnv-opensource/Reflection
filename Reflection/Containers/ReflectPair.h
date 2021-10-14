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
    template<typename First, typename Second>
    struct AutoReflectImplemented<std::pair<First,Second>, void>
    {
        static const bool value = true;
    };
}}}
namespace std 
{
    template<typename First, typename Second>
    void DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, pair<First, Second>**)
    {
        using namespace DNVS::MoFa::Reflection;
        using Tuple = pair<First, Second>;
		using NcFirst = remove_const_t<First>;
		using NcSecond = remove_const_t<Second>;
		using NonConstTuple = std::pair<NcFirst, NcSecond>;
		AutoReflect<NcFirst>::Reflect(typeLibrary);
		AutoReflect<NcSecond>::Reflect(typeLibrary);
		GenericTupleReflector<Tuple, NonConstTuple>::Reflect(typeLibrary);
    };
}