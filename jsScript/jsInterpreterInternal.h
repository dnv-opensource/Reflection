//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSINTERPRETERINTERNAL_H
#define JSINTERPRETERINTERNAL_H

#include <jsScript/jsScript_config.h>
#include <mofaTools/ref.h>
#include <jsScript/jsTypeLibrary.h>

namespace ixion {namespace javascript {class interpreter;}}
class jsScriptable;

class jsInterpreterInternal  
{
public:
    jsInterpreterInternal(jsTypeLibrary* typeLibraryJs);
	virtual ~jsInterpreterInternal();
   jsTypeLibrary& vtblMap();
private:
   jsTypeLibrary* m_vtblMap;
};

#endif