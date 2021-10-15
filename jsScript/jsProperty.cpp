//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsProperty.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsValue* jsProperty::setGet(jsPropertyGet* get)
{
   if(get) {
      typeInfo()->setType(typeInfo()->type()|jsTypeInfo::jsPropertyGet);
      typeInfo()->returned(get->propertyType());
      get->SetTypeInfo(typeInfo());
   }
   m_get=get;
   return this;
}

jsValue* jsProperty::setPut(jsPropertyPut* put)
{
   if(put) {
      put->SetTypeInfo(typeInfo());
      typeInfo()->setType(typeInfo()->type()|jsTypeInfo::jsPropertyPut);
      typeInfo()->returned(put->propertyType());
   }
   m_put=put;
   return this;
}

jsValue* jsProperty::duplicate(jsValue* owner)
{
   if(!m_get) throw std::exception("Property is Write-Only!");
   return m_get->duplicate(owner);
}

jsValue* jsProperty::lookup(const std::string& identifier,jsValue* owner) 
{
   return duplicate(owner)->lookup(identifier);
}

jsValue* jsProperty::assign(jsValue* owner,jsValue* op2)
{
   if(!m_put) throw std::exception("Property is Read-Only!");
   return m_put->assign(owner,op2);
}

jsValue* jsProperty::call(const Params& params)
{
   if(params.size()==1) return duplicate(params[0]);
   else if(params.size()==2) return assign(params[0],params[1]);
   else throw incorrect_number_of_arguments(params.size()-1);
}

std::string jsProperty::typeName()
{
   return "property";
}

void jsProperty::LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
{
    if(m_put)
        m_put->LogFunctionDetails(service);
    if(m_get)
        m_get->LogFunctionDetails(service);
}
