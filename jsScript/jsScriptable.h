//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSSCRIPTABLE_H
#define JSSCRIPTABLE_H

#include <jsScript/jsValue.h>
#include <map>

class jsVTBL;
class jsTypeLibrary;

struct jsUnion;
struct jsAutomation {};

class JSSCRIPT_IMPORT_EXPORT jsScriptable : public jsValue 
{
   friend class jsStack;
   friend class jsMember;
public:
	jsScriptable(const jsAutomation& automation);
	jsScriptable(const jsScriptable& object);
	virtual ~jsScriptable();
   jsValue* lookup(const std::string& identifier,jsValue* owner=NULL);
   bool methodName(jsValue* value,std::string& identifier);
   bool getName(std::string& name) const;
   bool init_union(std::list<jsValue*>& object_union);
   virtual jsValue* prototype();

   virtual std::string typeName();
protected:
   jsScriptable();
private:
    friend class jsTypeLibrary;
//Friend access only...
   void constructObject(jsTypeLibrary& typeLibrary);
   jsVTBL* m_vtbl;
   jsUnion* m_union;
};

#endif