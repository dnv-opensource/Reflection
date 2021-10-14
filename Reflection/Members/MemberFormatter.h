#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "IMember.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    std::string REFLECTION_IMPORT_EXPORT FormatSignature(const IMember& member, const TypeLibraries::TypeLibraryPointer& typeLibrary, const char* parenthesis = "()", bool includeNames = true);
}}}}
