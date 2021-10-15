//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef AFX_JSSTRINGORNUMBER_H
#define AFX_JSSTRINGORNUMBER_H

#include <jsScript/jsValue.h>
#include <jsScript/jsConversions.h>

template<typename T>
class jsStringOrNumber : public jsValue 
{
public:
   jsStringOrNumber(T number) 
   :  m_value(number)
   ,  m_isString(false)
   {}
   jsStringOrNumber(const std::string& value)
      :  m_isString(true)
      ,  m_string(value)
      ,  m_value(T())
   {}
   jsStringOrNumber(jsValue* value) {
      mofa::ref<jsValue> val(value->duplicate());
      m_isString=nativeType(val,jsType<std::string>());
      if(m_isString) m_string=fromJScript(value,jsType<std::string>());
      else m_value=fromJScript(value,jsType<T>());
   }
	virtual ~jsStringOrNumber() {};
   bool isString() const {return m_isString;}
   std::string getString() const {return m_string;}
   T getNumber() const {return m_value;}
   static void init(jsTypeLibrary& typeLibrary) {
      jsStack::stack()->addType(typeid(jsStringOrNumber),jsStack::stack()->typeName(typeid(T)) + " or " + jsStack::stack()->typeName(typeid(std::string)));
      jsTClass<jsStringOrNumber<T>> cls(typeLibrary);
      if(cls.reinit())
         return;
      cls.Constructor<T>();
      cls.Constructor<const std::string&>();
   }
private:
   std::string m_string;
   T m_value;
   bool m_isString;
};

template<typename T>
bool pointer_unwrap(jsValue* value,jsStringOrNumber<T>*& result,const jsStringOrNumber<T>* =0)
{  
   result=new jsStringOrNumber<T>(value);
   jsStack::stack()->insert(result);
   return true;
}

#endif
