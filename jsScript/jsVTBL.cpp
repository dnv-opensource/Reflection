//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsVTBL.h>
#include <jsScript/jsValue.h>
#include <jsScript/jsConversions.h>
#include <jsScript/jsConstructor.h>
#include <jsScript/jsFunction.h>
#include <jsScript/jsCollections.h>
#include <jsScript/jsProperty.h>
#include <jsScript/jsStack.h>
#include <jsScript/jsValueIterator.h>
#include <algorithm>
#include "jsTypeLibrary.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vtbl_not_found::vtbl_not_found(const type_info& info)
:  std::exception(errorMessage(info).c_str())
{

}

std::string vtbl_not_found::errorMessage(const type_info& info)
{
   std::string error="could not find VTBL for type \'" + jsStack::stack()->typeName(info) + "\'.";
   return error;
}


jsVTBL::jsVTBL(const type_info& info, jsTypeLibrary& typeLibrary,jsVTBL* parent)
:  m_parent(parent)
,  m_typeName(jsStack::stack()->typeName(info))
, m_typeInformation(info)
,  m_enabled(true)
,  m_typeLibrary(typeLibrary)
{
   m_typeInfo=new jsTypeInfo(this,m_typeName,jsTypeInfo::jsObject);
}

jsVTBL::~jsVTBL()
{
}

bool jsVTBL::isEnabled() const
{
    return m_enabled;
}

void jsVTBL::enable(bool flag)
{
    m_enabled=flag;
}

jsValue* jsVTBL::add(const std::string& identifier,jsValue* function)
{
   if(m_vtbl.find(identifier)!=m_vtbl.end()) {
      jsValue* value=m_vtbl[identifier];
      
      jsConstructor* c1=dynamic_cast<jsConstructor*>(function);
      jsConstructor* c2=dynamic_cast<jsConstructor*>(value);      
      if(c1 && c2) return addConstructorCollection(identifier,c1,c2);

      jsFunction* f1=dynamic_cast<jsFunction*>(function);
      jsFunction* f2=dynamic_cast<jsFunction*>(value);
      if(f1 && f2) return addFunctionCollection(identifier,f1,f2);

      jsProperty* p1=dynamic_cast<jsProperty*>(value);
      if(p1) {
         jsPropertyGet* pGet=dynamic_cast<jsPropertyGet*>(function);
         if(pGet) return p1->setGet(pGet);
         jsPropertyPut* pPut=dynamic_cast<jsPropertyPut*>(function);
         if(pPut) return p1->setPut(pPut);         
      }
         
      std::string errorMessage="Unable to overload member '" + typeName() +"."+identifier+"'";

      throw std::exception(errorMessage.c_str());

   }

   if(jsPropertyGet* pGet=dynamic_cast<jsPropertyGet*>(function)) {
      jsProperty* prop=new jsProperty(m_typeLibrary,identifier);
      function=prop->setGet(pGet);
   }
   else if(jsPropertyPut* pPut=dynamic_cast<jsPropertyPut*>(function)) {
      jsProperty* prop=new jsProperty(m_typeLibrary,identifier);
      function=prop->setPut(pPut);
   }      
      
   m_vtbl[identifier]=function;
   std::string lowerIdentifier=identifier;
   std::transform(lowerIdentifier.begin(),lowerIdentifier.end(),lowerIdentifier.begin(),tolower);
   m_caseless_vtbl[lowerIdentifier]=function;
   return function;
}

jsValue* jsVTBL::addConstructorCollection(const std::string& identifier,mofa::ref<jsConstructor> c1,mofa::ref<jsConstructor> c2)
{
   jsConstructorCollection* coll=dynamic_cast<jsConstructorCollection*>(c2.get());
   if(!coll) {
      coll=new jsConstructorCollection(m_typeLibrary);
      m_vtbl[identifier]=coll;

      std::string lowerIdentifier=identifier;
      std::transform(lowerIdentifier.begin(),lowerIdentifier.end(),lowerIdentifier.begin(),tolower);
      m_caseless_vtbl[lowerIdentifier]=coll;

      coll->add(c2);
   }
   coll->add(c1);
   return coll;
}

jsValue* jsVTBL::addFunctionCollection(const std::string& identifier,mofa::ref<jsFunction> f1,mofa::ref<jsFunction> f2)
{
   jsFunctionCollection* coll=dynamic_cast<jsFunctionCollection*>(f2.get());
   if(!coll) {
      coll=new jsFunctionCollection(m_typeLibrary);
      m_vtbl[identifier]=coll;

      std::string lowerIdentifier=identifier;
      std::transform(lowerIdentifier.begin(),lowerIdentifier.end(),lowerIdentifier.begin(),tolower);
      m_caseless_vtbl[lowerIdentifier]=coll;

      coll->add(f2);
   }
   coll->add(f1);
   return coll;
}

jsValue* jsVTBL::lookup(const std::string& identifier,jsValue* owner)
{
   if(!m_typeLibrary.IsCaseSensitive()) {
      std::string lcaseName=identifier;
      std::transform(lcaseName.begin(),lcaseName.end(),lcaseName.begin(),tolower);
      VTBL::iterator it=m_caseless_vtbl.find(lcaseName);
      if(it!=m_caseless_vtbl.end()) {
         return it->second;
      }
      if(m_union.size()>0) {
         for(union_iterator iunion=m_union.begin();iunion!=m_union.end();iunion++) {
            jsVTBL* vtbl=*iunion;
            it=vtbl->m_caseless_vtbl.find(lcaseName);
            if(it!=vtbl->m_caseless_vtbl.end()) {
               return it->second;
            }         
         }
      }
   }
   else {
      VTBL::iterator it=m_vtbl.find(identifier);
      if(it!=m_vtbl.end()) {
         return it->second;
      }
      if(m_union.size()>0) {
         for(union_iterator iunion=m_union.begin();iunion!=m_union.end();iunion++) {
            jsVTBL* vtbl=*iunion;
            it=vtbl->m_vtbl.find(identifier);
            if(it!=vtbl->m_vtbl.end()) {
               return it->second;
            }         
         }
      }
   }
   if(m_parent) return m_parent->lookup(identifier);
   else return NULL;
}   

bool jsVTBL::isParent(jsVTBL* vtbl)
{
   if(m_parent) {
      if(m_parent==vtbl) return true;
      return m_parent->isParent(vtbl);
   }
   return false;
}

bool jsVTBL::methodName(jsValue* value,std::string& identifier)
{
   VTBL::iterator it;
   for(it=m_vtbl.begin();it!=m_vtbl.end();it++) {
      if(it->second==value) {
         identifier=it->first;
         return true;
      }
   }
   if(m_union.size()>0) {
      for(union_iterator iunion=m_union.begin();iunion!=m_union.end();iunion++) {
         jsVTBL* vtbl=*iunion;
         for(it=vtbl->m_vtbl.begin();it!=vtbl->m_vtbl.end();it++) {
            if(it->second==value) {
               identifier=it->first;
               return true;
            }
         }
      }
   }
   if(m_parent) return m_parent->methodName(value,identifier);
   return false;
}

void jsVTBL::unite(jsVTBL* vtbl2)
{
   if(isParent(vtbl2) || vtbl2->isParent(this)) throw std::exception("Could not unite object with parent!");
   m_union.push_back(vtbl2);
   vtbl2->m_union.push_back(this);
}

jsVTBL::union_iterator jsVTBL::union_begin()
{
   return m_union.begin();
}

jsVTBL::union_iterator jsVTBL::union_end()
{
   return m_union.end();
}

size_t jsVTBL::union_size()
{
   return m_union.size();
}

std::string jsVTBL::typeName()
{
   return m_typeName;
}

void jsVTBL::LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
{
    for(auto it = m_vtbl.begin(); it != m_vtbl.end(); ++it)
        it->second->LogFunctionDetails(service);
}

jsVTBL::vtbl_iterator jsVTBL::vtbl_begin()
{
   return m_vtbl.begin();
}

jsVTBL::vtbl_iterator jsVTBL::vtbl_end()
{
   return m_vtbl.end();
}

size_t jsVTBL::vtbl_size()
{
   return m_vtbl.size();
}

jsValueIterator jsVTBL::child_begin() const
{
   return jsValueIterator(new iterator_ref_pair_holder<VTBL::const_iterator>(m_vtbl.begin()));
}

jsValueIterator jsVTBL::child_end() const
{
   return jsValueIterator(new iterator_ref_pair_holder<VTBL::const_iterator>(m_vtbl.end()));
}


size_t jsVTBL::child_size() const
{
   return m_vtbl.size();
}


jsValue* jsVTBL::prototype()
{
   return m_parent;
}

void jsVTBL::getTypeInfo(TypeInfoSet& members,int type,jsValue* parent)
{
   Union vtbls;
   getUnions(vtbls);
   for(union_iterator ivtbl=vtbls.begin();ivtbl!=vtbls.end();ivtbl++) {
      jsVTBL* vtbl=*ivtbl;
      for(vtbl_iterator it=vtbl->vtbl_begin();it!=vtbl->vtbl_end();it++) {
         std::string name=it->first;
         it->second->getTypeInfo(members,type,parent);
      }
   }
}

jsTypeInfo* jsVTBL::typeInfo()
{
   return m_typeInfo;
}

const type_info& jsVTBL::GetTypeInformation() const
{
	return m_typeInformation;
}

void jsVTBL::getUnions(Union &vtbls)
{
   if(std::find(begin(vtbls), end(vtbls), this)!=vtbls.end()) return;
   vtbls.push_back(this);
   for(union_iterator it=union_begin();it!=union_end();it++) {
      (*it)->getUnions(vtbls);
   }
}
