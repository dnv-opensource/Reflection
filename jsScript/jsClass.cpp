//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsClass.h>
#include <jsScript/jsTypeLibrary.h>
#include <jsScript/jsVTBL.h>
#include "jsModelObject.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsBaseClass::jsBaseClass(jsTypeLibrary& typeLibrary, const type_info& info, const type_info& parent, const std::string& name)
    : m_typeLibrary(typeLibrary)
    , m_exposeToScripting(true)
{
    if (name.size() > 0) m_typeLibrary.SetTypeName(info, name);
    m_reinit = m_typeLibrary.isEnabled(info);
    m_vtbl = m_typeLibrary.add(info, parent);
    auto type = typeLibrary.GetReflectionTypeLibrary()->CreateType(info, "");
    if (info != parent)
        type->AddBaseType(parent);
    else if (info != typeid(jsValue))
        type->AddBaseType(typeid(jsValue));

}

jsBaseClass::jsBaseClass(const jsBaseClass& cls)
    : m_exposeToScripting(cls.m_exposeToScripting)
    , m_typeLibrary(cls.m_typeLibrary)
{
    m_vtbl = cls.m_vtbl;
}

jsBaseClass& jsBaseClass::operator=(const jsBaseClass& cls)
{
    if (&cls != this) {
        m_vtbl = cls.m_vtbl;
        m_exposeToScripting = cls.m_exposeToScripting;
    }
    return *this;
}

void jsBaseClass::enableScripting(bool value)
{
    m_exposeToScripting = value;
    m_vtbl->enable(value);
}

bool jsBaseClass::isScriptingEnabled() const
{
    return m_exposeToScripting;
}

jsBaseClass::~jsBaseClass()
{

}

bool jsBaseClass::reinit()
{
    return m_reinit;
}

jsValue* jsBaseClass::add(const std::string& identifier, jsValue* value)
{
    return m_vtbl->add(identifier, value);
}

void jsBaseClass::addGlobal(const std::string& identifier, jsValue* value)
{
    m_typeLibrary.lookup(typeid(jsModelObject))->add(identifier, value);
}

jsValue* jsBaseClass::lookup(const std::string& identifier)
{
    return m_vtbl->lookup(identifier);
}

void jsBaseClass::unite(const type_info& type)
{
    jsVTBL* vtbl = m_typeLibrary.lookup(type);
    if (vtbl) m_vtbl->unite(vtbl);
}

jsTypeInfo* jsBaseClass::typeInfo()
{
    return m_vtbl->typeInfo();
}
#pragma warning(push)
#pragma warning(disable:4355)//'this' : used in base member initializer list
class jsDummyValue : public jsValue {
public:
    jsDummyValue() : m_info(this, "dummy", jsTypeInfo::jsAll, 100) {
        for (size_t i = 0; i < 100; ++i) {
            m_info.argument(i, "dummy");
            m_info.returned("dummy");
        }
    }
    jsTypeInfo* typeInfo() { return &m_info; }
private:
    jsTypeInfo m_info;
};
#pragma warning(pop)
jsValue* jsBaseClass::getDummyValue()
{
    static jsDummyValue dummyValue;
    return &dummyValue;
}
