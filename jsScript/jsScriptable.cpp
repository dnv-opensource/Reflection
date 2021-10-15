//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsScriptable.h>
#include <jsScript/jsTypeLibrary.h>
#include <jsScript/jsVTBL.h>
#include <jsScript/jsUnion.h>
#include <jsScript/jsConstructor.h>
#include <jsScript/jsFunction.h>
#include <jsScript/jsProperty.h>
#include "jsStack.h"
#include "Services/ServiceProvider.h"
#include "Scripting/IScriptingService.h"

using namespace DNVS::MoFa::Services;
using namespace DNVS::MoFa::Scripting;
using namespace DNVS::MoFa::Reflection::Objects;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsScriptable::jsScriptable(const jsAutomation&  automation)
    : m_vtbl(NULL)
    , m_union(NULL)
{

}

jsScriptable::jsScriptable()
    : m_vtbl(NULL)
    , m_union(NULL)
{
}
jsScriptable::jsScriptable(const jsScriptable& object)
    : m_vtbl(object.m_vtbl)
    , m_union(NULL)
{
}


jsScriptable::~jsScriptable()
{
    if (m_union) delete m_union;
}

jsValue* jsScriptable::lookup(const std::string& identifier, jsValue* owner)
{
    if (!prototype()) throw vtbl_not_found(typeid(*this));
    return prototype()->lookup(identifier);
}

void jsScriptable::constructObject(jsTypeLibrary& typeLibrary)
{
    jsVTBL* vtbl = typeLibrary.lookup(typeid(*this));
    if (vtbl) m_vtbl = vtbl;
    else throw vtbl_not_found(typeid(*this));
}

bool jsScriptable::methodName(jsValue* value, std::string& identifier)
{
    if (!m_vtbl) throw vtbl_not_found(typeid(*this));
    return m_vtbl->methodName(value, identifier);
}

bool jsScriptable::getName(std::string& name) const
{
    return ServiceProvider::Instance().GetService<IScriptingService>()->TryGetName(Object(jsStack::stack()->GetTypeLibrary(), this), name);
}

bool jsScriptable::init_union(std::list<jsValue*>& object_union)
{
    if (!m_vtbl) m_vtbl = jsStack::stack()->GetJsTypeLibrary().lookup(typeName());
    if (m_vtbl->union_size() == 0 && !m_union) return false;
    if (!m_union) m_union = new jsUnion(this);
    jsVTBL::union_iterator it;
    for (it = m_vtbl->union_begin(); it != m_vtbl->union_end(); it++) {
        if (m_union->find(*it) == m_union->end()) {
            jsVTBL* vtbl = *it;
            jsValue* value = vtbl->lookup("_unite_" + m_vtbl->typeName());
            if (!value) throw std::exception("could not find union constructor!");
            Params params;
            params.push_back(this);
            jsValue* union_value = value->construct(params);
            jsScriptable* union_scriptable = dynamic_cast<jsScriptable*>(union_value);
            if (!union_scriptable) throw std::exception("unable to unite objects!");
            union_scriptable->m_union = new jsUnion(this);
            union_scriptable->m_union->insert(jsUnion::value_type(m_vtbl, this));
            m_union->insert(jsUnion::value_type(vtbl, union_value));
        }
    }

    for (jsUnion::iterator iunion = m_union->begin(); iunion != m_union->end(); iunion++) {
        jsValue* val = iunion->second;
        object_union.push_back(val);
    }
    return true;
}

jsValue* jsScriptable::prototype()
{
    if (!m_vtbl)
        constructObject(jsStack::stack()->GetJsTypeLibrary());
    return m_vtbl;
}

std::string jsScriptable::typeName()
{
    if (!m_vtbl) return jsStack::stack()->typeName(typeid(*this));
    return m_vtbl->typeName();
}
