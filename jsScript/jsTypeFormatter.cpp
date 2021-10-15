//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsTypeFormatter.h"
#include "jsStack.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Types/DecoratedTypeFormatter.h"
using namespace DNVS::MoFa::Reflection;
using namespace Types;
using namespace TypeLibraries;
std::string jsTypeFormatter::FormatType(const DecoratedTypeInfo& typeInfo) const
{
    return jsStack::stack()->typeName(typeInfo.GetTypeInfo().name());
}

void JSSCRIPT_IMPORT_EXPORT InstallJsTypeFormatter(const TypeLibraryPointer& typeLibrary)
{
    auto fallbackTypeFormatter = std::make_shared<jsTypeFormatter>();
    auto typeLibraryFormatter = std::make_shared<TypeLibraryNamedTypeFormatter>(typeLibrary.get(), fallbackTypeFormatter);
    typeLibrary->SetTypeFormatter(std::make_shared<DecoratedTypeFormatter>(typeLibraryFormatter));
}
