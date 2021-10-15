//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSENUM_H
#define JSENUM_H


#include <jsScript/jsMember.h>

class jsEnumBase : public jsMember {
public:
   jsEnumBase(jsTypeLibrary& typeLibrary,const std::string& name,int trimLeft,bool smartSpaces)
   :  jsMember(typeLibrary,name,jsTypeInfo::jsPropertyGet) 
   ,  m_trimLeft(trimLeft)
   ,  m_smartSpaces(smartSpaces)
   {}
   jsEnumBase(jsTypeLibrary& typeLibrary) : jsMember(typeLibrary) {}
   virtual ~jsEnumBase() {}
   virtual long getEnumValue() const = 0;
   int trimLeft() const {return m_trimLeft;}
   bool smartSpaces() const {return m_smartSpaces;}
private:
   bool m_smartSpaces;
   int m_trimLeft;
};

template<class T>
class jsEnum : public jsEnumBase  
{
public:
   jsEnum(jsTypeLibrary& typeLibrary,const std::string& name,T value,int trimLeft=0,bool smartSpaces=false) :
      jsEnumBase(typeLibrary,name,trimLeft,smartSpaces) , m_value(value) 
   {   
      if(typeInfo()) typeInfo()->returned(jsStack::stack()->typeName(typeid(*this)));
   }
   jsEnum(T value) : jsEnumBase(jsStack::stack()->GetJsTypeLibrary()) , m_value(value) {}
   virtual ~jsEnum() {}
   operator T() const {return m_value;}
   virtual long getEnumValue() const {return (long)m_value;}
   jsValue* duplicate(jsValue* owner) 
   {
	   jsMemberLogger logger(this);
	   return this;
   }
   jsValue* assign(jsValue* owner,jsValue* op2) {throw std::exception("Property is Read-Only!");}
   std::string typeName(){return "property";}
   static void init(jsTypeLibrary& typeLibrary,const std::string& name) {
      jsTClass<jsEnum<T> > cls(typeLibrary,name);
      DNVS::MoFa::Reflection::AddClassWithBase<jsEnum<T>,jsEnumBase>(typeLibrary.GetReflectionTypeLibrary()->GetConversionGraph());
      cls.ImplicitConstructorConversion<T>(false);
      cls.ImplicitConversion(&jsEnum::operator T,false);
      typeLibrary.SetTypeName(typeid(T),name);
   }

   virtual jsValue* operator==(jsValue* op2)
   {
        return toJScript(equalToThis(op2));
   }

   virtual jsValue* operator!=(jsValue* op2)
   {
       return toJScript(!equalToThis(op2));
   }

private:

    bool equalToThis(jsValue* op2)
    {
        jsEnum<T>* other = dynamic_cast<jsEnum<T>*>(op2);
        if (other) 
            return (getEnumValue() == other->getEnumValue());
        else
            throw std::runtime_error("Invalid comparison of enum and incompatible type");
    }


    T m_value;
};

#endif
