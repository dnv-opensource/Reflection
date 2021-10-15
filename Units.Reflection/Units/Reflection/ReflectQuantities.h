#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\ITypeLibrary.h"
#include "Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    void UNITS_REFLECTION_IMPORT_EXPORT ReflectQuantities(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary);
    void UNITS_REFLECTION_IMPORT_EXPORT ReflectDynamicQuantities(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary);
}}}}
