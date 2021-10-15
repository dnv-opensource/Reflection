//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsReference.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsReference::jsReference(jsValue* value)
:  m_value(value)
{

}

jsReference::~jsReference()
{

}

jsValue* jsReference::lookup(const std::string& identifier,jsValue* owner)
{
   if(m_value) return m_value->lookup(identifier,owner);
   return NULL;
}

jsValue* jsReference::call(const Params& params)
{
   if(m_value) return m_value->call(params);
   return NULL;
}

jsValue* jsReference::duplicate(jsValue* owner)
{
   if(m_value) return m_value->duplicate(owner);
   return NULL;
}

jsValue* jsReference::value()
{
   return m_value;
}

jsValue* jsReference::assign(jsValue* owner,jsValue* op2)
{
   m_value=op2;
   return m_value;
}

jsValue* jsReference::construct(const Params& params)
{
   if(m_value) return m_value->construct(params);
   return NULL;   
}

jsValue* jsReference::prototype()
{
   if(m_value) return m_value->prototype();
   return NULL;
}

jsValue* jsReference::subscript(jsValue* index)
{
   if(m_value) return m_value->subscript(index);
   return NULL;
}