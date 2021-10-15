//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsModelObject.h>
#include <jsScript/jsClass.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsModelObject::~jsModelObject()
{

}

std::string Typeof(jsValue* value) {
    if(jsConstructor* member=dynamic_cast<jsConstructor*>(value)) 
        return member->return_value();
    else if(jsFunction* member=dynamic_cast<jsFunction*>(value)) 
        return member->return_value();
    else if(jsPropertyGet* member=dynamic_cast<jsPropertyGet*>(value)) 
        return member->propertyType();    
    else return value->typeName();
}

void jsModelObject::init(jsTypeLibrary& typeLibrary)
{
   jsTClass<jsModelObject> cls(typeLibrary,"Global");
   auto& info=cls.Function("typeof",Typeof);
   info.AddDocumentation("Return the type of an object or the return type of a function");
   info.AddSignature((DNVS::MoFa::Reflection::Members::Arg("objectOrMethod")));
   info.AddDocumentation("typeof(PipeSection); //returns \"PipeSection\"");
   info.AddDocumentation("sec=PipeSection(1,0.2);");
   info.AddDocumentation("typeof(sec); //returns \"PipeSection\"");
}
