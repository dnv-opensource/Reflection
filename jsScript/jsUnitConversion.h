//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef AFX_JSUNITCONVERSION_H
#define AFX_JSUNITCONVERSION_H

#include <jsScript/jsValue.h>
#include <jsScript/jsQuantity.h>

bool JSSCRIPT_IMPORT_EXPORT getUniqueQuantity(jsValue* value,jsQuantity& quantity);

#endif
