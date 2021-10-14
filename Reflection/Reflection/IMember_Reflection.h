#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "../Config.h"
#include "../TypeLibraries/TypeLibraryPointer.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    class IMember;
    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, IMember**);
}}}}
