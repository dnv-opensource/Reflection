//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsStackElement.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsStackElement::jsStackElement()
{

}

jsStackElement::~jsStackElement()
{

}

jsValue* jsStackElement::insert(jsValue* value)
{
   m_values.push_back(value);
   return value;
}

std::string& jsStackElement::insert(const std::string& value)
{
   m_strings.push_back(value);
   return *m_strings.rbegin();
}

void jsStackElement::insert(const DNVS::MoFa::Reflection::Variants::Variant& var)
{
    if(var.GetStorage()) m_storages.push_back(var.GetStorage());
}
