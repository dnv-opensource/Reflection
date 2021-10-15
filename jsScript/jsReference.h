//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSREFERENCE_H
#define JSREFERENCE_H

#include <jsScript/jsValue.h>

class JSSCRIPT_IMPORT_EXPORT jsReference : public jsValue
{
public:
   jsReference(jsValue* value=NULL);
   virtual ~jsReference();

   virtual jsValue* lookup(const std::string& identifier,jsValue* owner=NULL);
   virtual jsValue* call(const Params& params);
   virtual jsValue* duplicate(jsValue* owner=NULL);
   virtual jsValue* assign(jsValue* owner,jsValue* op2);
   virtual jsValue* construct(const Params& params);
   virtual jsValue* prototype();
   virtual jsValue* subscript(jsValue* index);
   jsValue* value();
private:
   mofa::ref<jsValue> m_value;
};

#endif 
