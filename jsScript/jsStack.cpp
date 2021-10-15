//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsStack.h>
#include <jsScript/jsTypeLibrary.h>
#include <jsScript/jsVTBL.h>
#include <jsScript/jsUnit.h>
#include <jsScript/jsUnitValue.h>
#include "jsTypeInfo.h"
#include "jsMember.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Classes/Class.h"
#include "Units/Runtime/UnitParser.h"
class jsArray;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsStack* jsStack::m_stack=NULL;
using namespace DNVS::MoFa::Units::Runtime;
jsStack::jsStack(jsTypeLibrary& typeLibrary)
    :  m_dummyMode(false)
    ,  m_constructionStack(0)
    , m_jstypeLibrary(typeLibrary)
{
    addType(typeid(void),"void");
    addType(typeid(std::string),"string");
    addType(typeid(std::string),"string");
    addType(typeid(const jsValue::Params&),"...");
    addType(typeid(jsValue),"Variant");
}

jsStack::~jsStack()
{

}

jsValue* jsStack::insert(jsValue* value)
{
    if(value==NULL) return NULL;
    return m_elements.top().insert(value);
}

std::string& jsStack::insert(const std::string& value)
{
    return m_elements.top().insert(value);
}

void jsStack::insert(const DNVS::MoFa::Reflection::Variants::Variant& var)
{
    m_elements.top().insert(var);
}

void jsStack::push()
{
    m_elements.push(jsStackElement());
}

void jsStack::pop()
{
    m_elements.pop();
}

bool jsStack::HasStack()
{
    return m_stack != nullptr;
}

bool jsStack::isBaseAndDerived(const type_info& base,const type_info& derived)
{
    jsValue* baseVTBL=GetJsTypeLibrary().lookup(typeName(base));
    jsValue* derivedVTBL= GetJsTypeLibrary().lookup(typeName(derived));
    if(!baseVTBL || !derivedVTBL) return false;
    while(derivedVTBL) {
        if(derivedVTBL==baseVTBL) return true;
        derivedVTBL=derivedVTBL->prototype();
    }
    return false;
}
jsValue* jsStack::construct(const type_info& type,jsValue* param)
{
    if(m_constructionStack>3) return NULL;
    m_constructionStack++;
    jsTypeLibrary& vtblMap= GetJsTypeLibrary();
    jsVTBL* vtbl=vtblMap.lookup(type);
    if(!vtbl || vtbl->typeName()=="Array") {
        m_constructionStack--;
        return NULL;
    }
    jsValue::Params params;

    params.push_back(NULL);
    params.push_back(param);

    //Get all constructors of a type.
    jsValue::TypeInfoSet constructors;
    vtbl->getTypeInfo(constructors,jsTypeInfo::jsConstructor);
    for(jsTypeInfo* info : constructors) {
        if(jsMember* member=dynamic_cast<jsMember*>(info->value())) {
            try {
                //Only consider constructors that are not explicit, since we are interested in an implicit conversion.
                if(member && !member->IsExplicit()) {
                    //Try to construct the object based on the input param.
                    if(jsValue* value=insert(member->construct(params))) {
                        m_constructionStack--;
                        return value;
                    }
                }
            }
            catch(...) {
            }
        }
    }
    m_constructionStack--;
    return NULL;
}

jsStack* jsStack::stack()
{
    if(!m_stack) 
        throw std::exception("Stack has not been initialised!");
    return m_stack;
}

void jsStack::setStack(jsStack* stack)
{
    m_stack=stack;
}

void jsStack::addDummy(const type_info& info,jsValue* value)
{
    std::string name=typeName(info);
    m_dummyObjects.insert(std::make_pair(name,value));
}

jsValue* jsStack::getDummy(const type_info& info,const type_info& owner)
{
    jsValue* value=NULL;
    if(value=getDummy(info)) return value;
    if(value=getDummy(owner)) return value;
    return NULL;
}

jsValue* jsStack::getDummy(const type_info& info)
{
    std::string name=typeName(info);
    DummyObjects::iterator it=m_dummyObjects.find(name);
    if(it!=m_dummyObjects.end()) return it->second;
    return NULL;
}

jsValue* jsStack::getDummy(const std::string& info)
{
    DummyObjects::iterator it=m_dummyObjects.find(info);
    if(it!=m_dummyObjects.end()) return it->second;
    return NULL;
}

bool jsStack::dummyMode()
{
    return m_dummyMode;
}

void jsStack::setDummyMode(bool dummyMode)
{
    m_dummyMode = dummyMode;
}

std::string jsStack::typeName(const type_info& info)
{
    return m_jstypeLibrary.GetTypeName(info);
}

std::string jsStack::typeName(const std::string& info)
{
    return m_jstypeLibrary.GetTypeName(info);
}

void jsStack::addType(const type_info& info,const std::string& name)
{
    return m_jstypeLibrary.SetTypeName(info, name);
}

jsUnit* jsStack::createUnit(const std::string& expression)
{
    auto unit = UnitParser::GetUnitParser().Parse(expression);
    if (!unit.IsValid())
        return nullptr;    
    jsUnit* jsunit=new jsUnit(unit);
    jsunit->constructObject(m_jstypeLibrary);
    return jsunit;
}

bool jsStack::validUnit(const std::string& expression)
{
    auto unit = UnitParser::GetUnitParser().Parse(expression);
    return unit.IsValid();
}

jsUnitValue* jsStack::createUnitValue(double value,jsUnit* unit)
{
    if(!unit) return NULL;
    jsUnitValue* jsunitvalue=new jsUnitValue(value,unit);
    jsunitvalue->constructObject(m_jstypeLibrary);
    return jsunitvalue;
}

void jsStack::SetTypeLibrary(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, bool registerIntegerDivide)
{
    m_typeLibrary=typeLibrary;
    if(!m_typeLibrary) return;
    using namespace DNVS::MoFa::Reflection::Classes;
    Class<bool> boolClass(typeLibrary,"bool");
    boolClass.Operator(!This.Const);
    boolClass.Operator(This.Const==This.Const);
    boolClass.Operator(This.Const!=This.Const);

    Class<double> doubleClass(typeLibrary,"double");
    doubleClass.Operator(This.Const==This.Const);
    doubleClass.Operator(This.Const!=This.Const);
    doubleClass.Operator(This.Const<This.Const);
    doubleClass.Operator(This.Const>This.Const);
    doubleClass.Operator(This.Const==This.Const);
    doubleClass.Operator(This.Const!=This.Const);
    doubleClass.Operator(This.Const<=This.Const);
    doubleClass.Operator(This.Const>=This.Const);
    doubleClass.Operator(This.Const+This.Const);
    doubleClass.Operator(This.Const-This.Const);
    doubleClass.Operator(-This.Const);
    doubleClass.Operator(+This.Const);
    doubleClass.Operator(This.Const*This.Const);
    doubleClass.Operator(This.Const/This.Const);

    Class<int> intClass(typeLibrary,"int");
    intClass.Operator(This.Const==This.Const);
    intClass.Operator(This.Const!=This.Const);
    intClass.Operator(This.Const<This.Const);
    intClass.Operator(This.Const>This.Const);
    intClass.Operator(This.Const==This.Const);
    intClass.Operator(This.Const!=This.Const);
    intClass.Operator(This.Const<=This.Const);
    intClass.Operator(This.Const>=This.Const);
    intClass.Operator(This.Const+This.Const);
    intClass.Operator(This.Const-This.Const);
    intClass.Operator(-This.Const);
    intClass.Operator(+This.Const);
    intClass.Operator(!This.Const);
    intClass.Operator(~This.Const);
    intClass.Operator(This.Const*This.Const);
    if(registerIntegerDivide)
        intClass.Operator(This.Const/This.Const);
}

DNVS::MoFa::Reflection::TypeConversions::ConversionGraphPointer jsStack::GetConversionGraph()
{
    if(m_typeLibrary) return m_typeLibrary->GetConversionGraph();
    else return DNVS::MoFa::Reflection::TypeConversions::ConversionGraphPointer();
}
DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer jsStack::GetTypeLibrary()
{
    return m_typeLibrary;
}

jsTypeLibrary& jsStack::GetJsTypeLibrary()
{
    return m_jstypeLibrary;
}

jsStackPusher::jsStackPusher()
{
    if(jsStack::stack()) jsStack::stack()->push();
}

jsStackPusher::~jsStackPusher()
{
    if(jsStack::stack()) jsStack::stack()->pop();
}
