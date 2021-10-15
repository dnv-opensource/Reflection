//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsValueWithOwner.h"



jsValue* jsValueWithOwner::Wrap(jsValue* value, jsValue* owner)
{
    if(!value)
        return nullptr;
    if(dynamic_cast<jsValueWithOwner*>(value))
        return value;
    else 
        return new jsValueWithOwner(value, owner);
}

jsValueWithOwner::jsValueWithOwner(jsValue* value, jsValue* owner)
    :   m_value(value)
    ,   m_owner(owner)
{

}

jsValue* jsValueWithOwner::lookup(const std::string& identifier,jsValue* owner)
{
    return m_value->lookup(identifier, m_owner);
}

jsValue* jsValueWithOwner::duplicate(jsValue* owner)
{
    return m_value->duplicate(m_owner);
}

jsValue* jsValueWithOwner::assign(jsValue* owner,jsValue* op2)
{
    return m_value->assign(m_owner, op2);
}

std::string jsValueWithOwner::typeName()
{
    return m_value->typeName();
}
