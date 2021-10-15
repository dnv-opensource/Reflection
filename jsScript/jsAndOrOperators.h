//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef AFX_JSANDOROPERATORS_H
#define AFX_JSANDOROPERATORS_H

#include <jsScript/jsScript_config.h>
#include <jsScript/jsValue.h>
#include <Reflection/TypeLibraries/TypeLibraryPointer.h>

class JSSCRIPT_IMPORT_EXPORT jsAndOrOperators  
{
public:
	jsAndOrOperators();
	virtual ~jsAndOrOperators();
   virtual jsValue* operator&&(jsAndOrOperators* other) = 0;
   virtual jsValue* operator||(jsAndOrOperators* other) = 0;
};

void JSSCRIPT_IMPORT_EXPORT DoReflect(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, jsAndOrOperators**);
inline bool pointer_unwrap(jsValue* value,jsAndOrOperators*& result,const jsAndOrOperators* output)
{
   result=dynamic_cast<jsAndOrOperators*>(value);
   if(result) return true;
   else return false;
}

#endif
