//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSSTACKELEMENT_H
#define JSSTACKELEMENT_H

#include <jsScript/jsScript_config.h>
#include <mofaTools/ref.h>
#include <string>
#include <list>

#include <jsScript/jsValue.h>
#include "Reflection/Variants/Variant.h"

class JSSCRIPT_IMPORT_EXPORT jsStackElement  
{
public:
	jsStackElement();
	virtual ~jsStackElement();

   jsValue* insert(jsValue* value);
   std::string& insert(const std::string& value);
   void insert(const DNVS::MoFa::Reflection::Variants::Variant& var);
private:
   std::list<std::string> m_strings;
   std::list<mofa::ref<jsValue> > m_values;
   std::list<DNVS::MoFa::Reflection::Variants::StoragePointer> m_storages;
};

#endif