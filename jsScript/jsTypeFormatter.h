#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/ITypeFormatter.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "jsScript_config.h"

class JSSCRIPT_IMPORT_EXPORT jsTypeFormatter : public DNVS::MoFa::Reflection::Types::ITypeFormatter
{
public:
    std::string FormatType(const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& typeInfo) const override;
};

void JSSCRIPT_IMPORT_EXPORT InstallJsTypeFormatter(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary);