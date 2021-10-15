//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "jsAndOrOperators.h"
#include "Reflection\Classes\Class.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsAndOrOperators::jsAndOrOperators()
{

}

jsAndOrOperators::~jsAndOrOperators()
{

}

void DoReflect(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, jsAndOrOperators**)
{
    using namespace DNVS::MoFa::Reflection::Classes;
    Class<jsAndOrOperators> cls(typeLibrary, "");
}
