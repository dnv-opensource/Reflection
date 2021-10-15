//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSVALUE_H
#define JSVALUE_H

#pragma warning(disable:4786)
#include <jsScript/jsScript_config.h>
#include <string>
#include <vector>
#include <set>
#include <list>

#include <mofaTools/ref.h>
#include <memory>
#include "TypeUtilities/IntrusiveClass.h"


class jsTypeInfo;
class jsMemberInfo;
class jsTypeLibrary;

struct jsValueIterator;
namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class IFunctionDetailService;
}}}

class JSSCRIPT_IMPORT_EXPORT jsValue : public DNVS::MoFa::TypeUtilities::IntrusiveClass<jsValue>
{
public:
    jsValue();
    virtual ~jsValue();
    typedef std::vector<jsValue*> Params;

    virtual jsValue* lookup(const std::string& identifier,jsValue* owner=NULL);
    virtual jsValue* call(const Params& params);
    virtual jsValue* duplicate(jsValue* owner=NULL);
    virtual jsValue* assign(jsValue* owner,jsValue* op2);
    virtual jsValue* construct(const Params& params);
    virtual bool methodName(jsValue* value,std::string& identifier) {return false;}
    virtual jsValue* prototype();
    virtual jsValue* subscript(jsValue* index);

    virtual jsValue* add(const std::string& identifier,jsValue* value);

    virtual std::string typeName();

    virtual bool getName(std::string& name) const;
    virtual void setName(const std::string& name);
    virtual void deleteObject();

    virtual bool init_union(std::list<jsValue*>& object_union);
    virtual jsTypeInfo* typeInfo();

    typedef std::vector<mofa::ref<jsTypeInfo>> TypeInfoSet;

    virtual void getTypeInfo(TypeInfoSet& members,int type,jsValue* parent=NULL);
    //Operators
    virtual jsValue* operator!();
    virtual jsValue* operator-();
    virtual jsValue* operator+(jsValue* op2);
    virtual jsValue* operator-(jsValue* op2);
    virtual jsValue* operator*(jsValue* op2);
    virtual jsValue* operator/(jsValue* op2);
    virtual jsValue* operator==(jsValue* op2);
    virtual jsValue* operator!=(jsValue* op2);
    virtual jsValue* operator<=(jsValue* op2);
    virtual jsValue* operator>=(jsValue* op2);
    virtual jsValue* operator<(jsValue* op2);
    virtual jsValue* operator>(jsValue* op2);
    virtual jsValue* operator+=(jsValue* op2);
    virtual jsValue* operator-=(jsValue* op2);
    virtual jsValue* operator*=(jsValue* op2);
    virtual jsValue* operator/=(jsValue* op2);
    ///Executes the operation op2/op1. Needed due to problems with the javascript parser.
    virtual jsValue* InverseDivide(jsValue* op2);

    virtual jsValueIterator child_begin() const;
    virtual jsValueIterator child_end() const;
    virtual size_t          child_size() const;
    virtual void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service);
};

#endif
