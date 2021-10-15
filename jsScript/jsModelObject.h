//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMODELOBJECT_H
#define JSMODELOBJECT_H

#include <jsScript/jsScriptable.h>

class JSSCRIPT_IMPORT_EXPORT jsModelObject : public jsScriptable  
{
public:
   jsModelObject() {}
	virtual ~jsModelObject();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif