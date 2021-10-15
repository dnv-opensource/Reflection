//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include<jsScript/jsArray.h>
#include<jsScript/jsReference.h>
#include<jsScript/jsClass.h>
#include<jsScript/jsValueIterator.h>
#include "jsArrayToContainerAlternativeConverter.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Objects/Delegate.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsArray::jsArray(const Params& params)
:  m_vector(params.size())
{
   for(size_t i=0;i<m_vector.size();i++) {
      m_vector[i]=new jsReference(params[i]);
   }
}

jsArray::jsArray(unsigned int length)
:  m_vector(length)
{
   for(size_t i=0;i<m_vector.size();i++) {
      m_vector[i]=new jsReference();
   }
}

jsArray::~jsArray()
{

}

jsValue* jsArray::subscript(jsValue* index)
{
   return operator[](fromJScript(index,jsType<int>()));
}

jsValue* jsArray::operator[](unsigned int index) const
{
   if(index < m_vector.size()) return m_vector[index];
   else throw std::exception("Index outside array bounds");
}

jsValue* jsArray::operator[](unsigned int index)
{
   if(index < m_vector.size()) return m_vector[index];
   else {
      size_t size=m_vector.size();
      m_vector.resize(index+1);
      for(size_t i=size;i<=index;i++) {
         m_vector[i]=new jsReference();
      }
   }
   return m_vector[index];
}


unsigned int jsArray::push(const Params& params)
{
   for(size_t i=0;i<params.size();i++) {
      m_vector.push_back(new jsReference(params[i]));
   }
   return (unsigned)m_vector.size();
}

jsValue* jsArray::pop()
{
   if(m_vector.size()==NULL) return NULL;
   jsValue* value=*m_vector.rbegin();
   jsStack::stack()->insert(value);
   m_vector.pop_back();
   return value;
}

unsigned int jsArray::get_length() const
{
   return (unsigned)m_vector.size();
}

void jsArray::put_length(unsigned int length)
{
   size_t size=m_vector.size();
   if(size>length) {
      m_vector.erase(m_vector.begin()+(length-1),m_vector.end());
   }
   else {
      m_vector.resize(length);
      for(size_t i=size;i<=length;i++) {
         m_vector[i]=new jsReference();
      }
   }
}

void jsArray::init(jsTypeLibrary& typeLibrary, bool RegisterConstructor)
{
   jsTClass<jsArray> cls(typeLibrary,"Array");
   if(cls.reinit()) return;

   cls.AddDocumentation("");
   cls.AddDocumentation("Create an array or control the program flow");

   //Examples
   auto& example=cls.AddExample("Creating braces in the jacket wizard");
   example.AddScript("for (i = 0; i < (NrxBrace); i++) ");
   example.AddScript("{");
   example.AddScript("&nbsp;&nbsp;Thisbrace = xBrace[i];");
   example.AddScript("&nbsp;&nbsp;if (Thisbrace == \"braceup\") ");
   example.AddScript("&nbsp;&nbsp;{");
   example.AddScript("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;BraceUpX(i); ");
   example.AddScript("&nbsp;&nbsp;}");
   example.AddScript("&nbsp;&nbsp;else if (Thisbrace == \"bracedown\") ");
   example.AddScript("&nbsp;&nbsp;{");
   example.AddScript("&nbsp;&nbsp;&nbsp;&nbsp;BraceDownX(i); ");
   example.AddScript("&nbsp;&nbsp;}");
   example.AddScript("&nbsp;&nbsp;else if (Thisbrace == \"xbrace\") ");
   example.AddScript("&nbsp;&nbsp;{ ");
   example.AddScript("&nbsp;&nbsp;&nbsp;&nbsp;BraceUpX(i); ");
   example.AddScript("&nbsp;&nbsp;&nbsp;&nbsp;BraceDownX(i); ");
   example.AddScript("&nbsp;&nbsp;}");
   example.AddScript("&nbsp;&nbsp;else if (Thisbrace == \"kbrace\") ");
   example.AddScript("&nbsp;&nbsp;{ ");
   example.AddScript("&nbsp;&nbsp;&nbsp;&nbsp;Print(\"K-Brace not implemented\"); ");
   example.AddScript("&nbsp;&nbsp;}");
   example.AddScript("}");
   example.AddComment("xBrace[i] is an array of the braces to be created");
   example.AddComment("BraceUpX(i), BraceDownX(i) etc. are functions that create braces");

   auto& example2=cls.AddExample("Create an array of points on a circle");
   example2.AddScript("var myPoints = new Array(NrPoints);");
   example2.AddScript("var NrPoints = 36;");
   example2.AddScript("var Rad = 10.0; ");
   example2.AddScript("var X,Y; ");
   example2.AddScript("for(var icount = 0; icount < NrPoints; icount++)");
   example2.AddScript("{");
   example2.AddScript("&nbsp;&nbsp;X=Rad*Math.cos(icount*10*Math.PI/180);");
   example2.AddScript("&nbsp;&nbsp;Y=Rad*Math.sin(icount*10*Math.PI/180);");
   example2.AddScript("&nbsp;&nbsp;myPoints[icount]=Point(X,Y,0);");
   example2.AddScript("&nbsp;&nbsp;myPoints[icount].name=\"Point\"+icount;");
   example2.AddScript("}");
   example2.AddComment("//var NrPoints = Number of points");
   example2.AddComment("//var X, Y = X, Y coordinate");
   example2.AddComment("//var Rad = Radius");

   auto& example3=cls.AddExample("Set X-coordinate of some points");
   example3.AddScript("var MyPoints = new Array();");
   example3.AddScript("MyPoints[0] = Point(0, 0, 1.0);");
   example3.AddScript("MyPoints[1] = Point(0, 0, 2.0);");
   example3.AddScript("MyPoints[2] = Point(0, 0, 3.0);");
   example3.AddScript("for (var MyPoint in MyPoints)");
   example3.AddScript("{");
   example3.AddScript("&nbsp;&nbsp;MyPoint.x = 2m;");
   example3.AddScript("}");

   if(RegisterConstructor)
       cls.Constructor<const Params&>();
   cls.Get("length",&jsArray::get_length);
   cls.Set("length",&jsArray::put_length);
   cls.Function("push",&jsArray::push);
   cls.Function("pop",&jsArray::pop);
   typeLibrary.GetReflectionTypeLibrary()->GetConversionGraph()->AddAlternativeConverter(
       DNVS::MoFa::Reflection::Types::TypeId<jsArray*>(), std::make_shared<jsArrayToContainerAlternativeConverter>(jsStack::stack()->GetTypeLibrary()));
   typeLibrary.GetReflectionTypeLibrary()->GetConversionGraph()->AddAlternativeConverter(
       DNVS::MoFa::Reflection::Types::TypeId<void>(),
       std::make_shared<AlternativeContainerTojsArrayConverter>(typeLibrary.GetReflectionTypeLibrary())
   );
}

jsValueIterator jsArray::child_begin() const
{
   return jsValueIterator(new iterator_ref_holder<std::vector<mofa::ref<jsValue> >::const_iterator>(m_vector.begin()));
}

jsValueIterator jsArray::child_end() const
{
   return jsValueIterator(new iterator_ref_holder<std::vector<mofa::ref<jsValue> >::const_iterator>(m_vector.end()));
}

size_t          jsArray::child_size() const
{
   return m_vector.size();
}
