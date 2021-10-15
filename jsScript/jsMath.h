//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMATH_H
#define JSMATH_H


#include <jsScript/jsScriptable.h>
#include <jsScript/jsScript_config.h>
class jsUnitValue;
class jsQuantity;

class JSSCRIPT_IMPORT_EXPORT jsMath
{
public:
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
