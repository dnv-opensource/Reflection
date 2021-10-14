#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include <string>
#include <sstream>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    /// <summary>
    /// Registers type conversions and ToString methods for fundamental types
    /// </summary>
    class REFLECTION_IMPORT_EXPORT DefaultRegistration
    {
    public:
        static void Reflect(TypeLibraries::TypeLibraryPointer typeLibrary);
    };
}}}}
