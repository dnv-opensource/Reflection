//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <jsScript/jsValue.h>
#include <jsScript/jsTypeInfo.h>
#include <jsScript/jsStack.h>
#include <jsScript/jsValueIterator.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
jsValue::jsValue()
{
}

jsValue::~jsValue()
{
}


jsValue* jsValue::lookup(const std::string& identifier,jsValue* owner)
{
    return nullptr;
}

jsValue* jsValue::call(const std::vector<jsValue*>& params)
{
    return nullptr;
}

jsValue* jsValue::duplicate(jsValue* owner)
{
    return this;
}

jsValue* jsValue::subscript(jsValue* index)
{
    std::string error="Type '" + jsStack::stack()->typeName(typeid(*this)) + "' does not support subscript [] operators";
    throw std::exception(error.c_str());
}

jsValue* jsValue::assign(jsValue* owner,jsValue* op2)
{
    return NULL;
}

jsValue* jsValue::construct(const Params& params)
{
    return NULL;//Invalid construction!
}

void jsValue::setName(const std::string& name)
{
}

bool jsValue::getName(std::string& name) const
{
    return false;
}

std::string jsValue::typeName()
{
    return "jsValue";
}

bool jsValue::init_union(std::list<jsValue*>& object_union)
{
    return false;
}

void jsValue::deleteObject()
{
}

jsValue* jsValue::prototype()
{
    return NULL;
}

jsTypeInfo* jsValue::typeInfo()
{
    return NULL;
}

void jsValue::getTypeInfo(TypeInfoSet& members,int type,jsValue* parent)
{
    if(prototype()) {
        if(type&jsTypeInfo::jsObject) {
            members.push_back(prototype()->typeInfo());
        }
        prototype()->getTypeInfo(members,type,parent);
    }
}

jsValue* jsValue::add(const std::string& identifier,jsValue* value)
{
    return NULL;
}

jsValue* jsValue::operator!()
{
    throw std::exception();
}

jsValue* jsValue::operator-()
{
    throw std::exception();
}

jsValue* jsValue::operator+(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator-(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator*(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator+=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator-=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator*=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator/=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::InverseDivide(jsValue* op2)
{
    throw std::runtime_error("Inverse divide is not implemented for " + std::string(typeid(*this).name()));
}

jsValue* jsValue::operator/(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator==(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator!=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator<=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator>=(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator<(jsValue* op2)
{
    throw std::exception();
}

jsValue* jsValue::operator>(jsValue* op2)
{
    throw std::exception();
}

jsValueIterator jsValue::child_begin() const
{
    throw std::exception();
}

jsValueIterator jsValue::child_end() const
{
    throw std::exception();
}

size_t         jsValue::child_size() const
{
    return 0;
}

void jsValue::LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
{    
}
