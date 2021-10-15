//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsInterpreterInternal.h>
#include <jsScript/jsScriptable.h>
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "jsStack.h"
#include "jsTypeConverter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsInterpreterInternal::jsInterpreterInternal(jsTypeLibrary* typeLibrary)
    : m_vtblMap(typeLibrary)
{
}

jsInterpreterInternal::~jsInterpreterInternal()
{
}

jsTypeLibrary& jsInterpreterInternal::vtblMap()
{
   return *m_vtblMap;
}

std::string FormatType(const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& type)
{
    return jsStack::stack()->GetTypeLibrary()->GetTypeFormatter()->FormatType(type);
}