//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsTypeLibrary.h>
#include <jsScript/jsVTBL.h>
#include <jsScript/jsConversions.h>
#include <jsScript/jsStack.h>
#include "boost/algorithm/string/case_conv.hpp"
#include "jsTypeFormatter.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsTypeLibrary::jsTypeLibrary(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& reflectionTypeLibrary)
: m_reflectionTypeLibrary(reflectionTypeLibrary)
{
    InstallJsTypeFormatter(reflectionTypeLibrary);
}

jsTypeLibrary::~jsTypeLibrary()
{
   VTBLMap::iterator it;
   for(it=m_vtblMap.begin();it!=m_vtblMap.end();it++) {
      delete it->second;
   }
}

void jsTypeLibrary::SetTypeName(const type_info& info, const std::string& name)
{
    std::string oldName = GetTypeName(info);
    if (oldName != name) 
        m_nameMap.insert(std::make_pair(oldName, name));
}

std::string jsTypeLibrary::GetTypeName(const type_info& info) const
{
    return GetTypeName(info.name());
}

void jsTypeLibrary::replace(std::string& name, const std::string& problem, const std::string& substitute)
{
    size_t npos = name.size();
    size_t  pos = name.find(problem);
    while (pos < npos)
    {  // found an occurence of "problem" in "name"
        std::string tmp = name;
        name = tmp.replace(pos, problem.size(), substitute);
        npos = name.size();
        pos = name.find(problem);
    }
}

std::string jsTypeLibrary::GetTypeName(const std::string& info) const
{
    std::string typespec = info;
    replace(typespec, "class ", "");
    replace(typespec, "struct ", "");
    replace(typespec, "const ", "");
    replace(typespec, "enum ", "");
    replace(typespec, " &", "");
    replace(typespec, " *", "");
    replace(typespec, " __ptr64", "");
    replace(typespec, "ixion::javascript::", "");
    auto it = m_nameMap.find(typespec);
    if (it != m_nameMap.end()) 
        return it->second;
    else return typespec;
}

jsVTBL* jsTypeLibrary::add(const type_info& type,const type_info& parent_type)
{
   VTBLMap::iterator it;
   it=m_vtblMap.find(jsStack::stack()->typeName(type));
   //If type is already added, return it.
   if(it!=m_vtblMap.end()) {
       jsVTBL* vtbl=it->second;
       vtbl->enable(true);
       return vtbl;
   }
   jsVTBL* newType=NULL;
   
   //Create a base class VTBL
   if(parent_type==type) {
      newType=new jsVTBL(type,*this);
   }

   //Create a derived class VTBL
   else {
      jsVTBL* parent=lookup(parent_type);
      if(!parent) throw vtbl_not_found(parent_type);
      newType=new jsVTBL(type,*this,parent);
   }
   m_vtblMap[jsStack::stack()->typeName(type)]=newType;
   return newType;
}

jsVTBL* jsTypeLibrary::lookup(const type_info& type) const
{
   VTBLMap::const_iterator it;
   it=m_vtblMap.find(jsStack::stack()->typeName(type));
   if(it==m_vtblMap.end()) return NULL;
   return it->second;
}

void jsTypeLibrary::enable(const type_info& type,bool flag)
{
    jsVTBL* vtbl=lookup(type);
    if(vtbl) return vtbl->enable(flag);
}

bool jsTypeLibrary::isEnabled(const type_info& info) const
{
    jsVTBL* vtbl=lookup(info);
    if(!vtbl) return false;
    return vtbl->isEnabled();
}

void jsTypeLibrary::LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
{
    for(auto it = m_vtblMap.begin(); it != m_vtblMap.end(); ++it)
    {
        jsVTBL* vtbl = it->second;
        vtbl->LogFunctionDetails(service);
    }
}

bool jsTypeLibrary::IsCaseSensitive() const
{
    return true;
}

const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& jsTypeLibrary::GetReflectionTypeLibrary() const
{
    return m_reflectionTypeLibrary;
}

void jsTypeLibrary::addReservedWord(const std::string& word)
{
    m_caseLessReservedWords.insert(boost::algorithm::to_lower_copy(word));
    m_reservedWords.insert(word);
}

bool jsTypeLibrary::isReservedWord(const std::string& word) const
{
    if (!IsCaseSensitive()) {
        return m_caseLessReservedWords.find(boost::algorithm::to_lower_copy(word)) != m_caseLessReservedWords.end();
    }
    else {
        return m_reservedWords.find(word) != m_reservedWords.end();
    }
}

const jsTypeLibrary::VTBLMap& jsTypeLibrary::GetVTBLMap() const
{
	return m_vtblMap;
}

jsVTBL* jsTypeLibrary::lookup(const std::string& typeName) const
{
   VTBLMap::const_iterator it;
   it=m_vtblMap.find(typeName);
   if(it==m_vtblMap.end()) return NULL;
   return it->second;
}


void jsTypeLibrary::unite(const type_info& type1,const type_info& type2)
{
   jsVTBL* vtbl1=lookup(type1);
   if(!vtbl1) throw vtbl_not_found(type1);

   jsVTBL* vtbl2=lookup(type2);
   if(!vtbl1) throw vtbl_not_found(type2);

   vtbl1->unite(vtbl2);
}