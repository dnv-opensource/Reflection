//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsUnion.h>
#include <mofaTools/ref.h>
#include <jsScript/jsValue.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsUnion::jsUnion(jsValue* owner)
:  m_owner(owner)
{
}

jsUnion::~jsUnion()
{
   for(Union::iterator it=begin();it!=end();it++) {
      if(it->second!=m_owner) {
         //Convenient way to delete this union object.
         mofa::ref<jsValue> delete_this=it->second;
      }
   }
}

jsUnion::Union::iterator jsUnion::find(jsVTBL* vtbl)
{
   return m_union.find(vtbl);
}

jsUnion::Union::iterator jsUnion::end()
{
   return m_union.end();
}

jsUnion::Union::iterator jsUnion::begin()
{
   return m_union.begin();
}

size_t jsUnion::size()
{
   return m_union.size();
}

std::pair<jsUnion::iterator, bool> jsUnion::insert(const value_type& x)
{
   return m_union.insert(x);
}
