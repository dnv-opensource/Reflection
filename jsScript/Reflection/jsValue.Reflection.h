#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsScript\jsQuantity.h"
#include "Reflection\TypeLibraries\TypeLibraryPointer.h"
#include "jsScript\jsUnitValue.h"
#include "..\jsScript_config.h"

void JSSCRIPT_IMPORT_EXPORT DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, jsValue**);
void JSSCRIPT_IMPORT_EXPORT DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, jsScriptable**);
void JSSCRIPT_IMPORT_EXPORT DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, jsQuantity**);
void JSSCRIPT_IMPORT_EXPORT DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, jsUnitValue**);