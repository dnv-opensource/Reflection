//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsConversions.h>
#include <jsScript/jsMember.h>
#include <jsScript/jsScriptable.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsMember::jsMember(jsTypeLibrary& typeLibrary, const std::string& name,int type)
:  m_typeLibrary(typeLibrary)
{
   m_typeInfo=new jsTypeInfo(this,name,type);
}

jsMember::jsMember(jsTypeLibrary& typeLibrary)
:  m_typeLibrary(typeLibrary)
,  m_typeInfo(NULL)
{
}

jsMember::~jsMember()
{

}

void jsMember::registerObject(jsScriptable* object)
{
   if(object) object->constructObject(m_typeLibrary);
}

jsValue* jsMember::verify(jsValue* value)
{
   jsScriptable* object=dynamic_cast<jsScriptable*>(value);
   if(object && object->m_vtbl==NULL) registerObject(object);
   return value;
}

void jsMember::getTypeInfo(TypeInfoSet& members,int type,jsValue* parent)
{
   if(m_typeInfo && (m_typeInfo->type() & type) ) members.push_back(m_typeInfo);
}

jsTypeLibrary& jsMember::GetTypeLibrary()
{
    return m_typeLibrary;
}

const jsTypeLibrary& jsMember::GetTypeLibrary() const
{
    return m_typeLibrary;
}

jsTypeInfo* jsMember::typeInfo()
{
   return m_typeInfo;
}

void jsMember::SetTypeInfo(jsTypeInfo* info)
{
    m_typeInfo = info;
}

std::string jsMember::toString()
{
   return typeInfo()->name();
}

const type_info& jsMember::class_type() const
{
    return typeid(void);
}

using namespace DNVS::MoFa::Reflection::Types;
DecoratedTypeInfo jsMember::GetParamType(int i) const
{
    return DecoratedTypeInfo();
}
