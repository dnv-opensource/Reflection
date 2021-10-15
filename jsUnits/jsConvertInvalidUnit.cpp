//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "jsConvertInvalidUnit.h"
#include "jsLength.h"
#include "jsMass.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void convert_invalid_unit_test() {
   std::string warning;
   jsConvertInvalidUnit<jsLength,jsMass>(0,warning);
}