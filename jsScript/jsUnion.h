//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSUNION_H
#define JSUNION_H

#pragma warning(disable:4786)
#include <jsScript/jsScript_config.h>
#include <map>
class jsVTBL;
class jsValue;

struct jsUnion  
{
   jsUnion(jsValue* owner);
   ~jsUnion();
   typedef std::map<jsVTBL*,jsValue*> Union;
   typedef Union::iterator iterator;
   typedef Union::value_type value_type;

   Union::iterator find(jsVTBL* vtbl);
   Union::iterator end();
   Union::iterator begin();
   size_t size();
   std::pair<iterator, bool> insert(const value_type& x);
   Union m_union;
   jsValue* m_owner;
};

#endif