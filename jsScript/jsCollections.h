//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSCOLLECTIONS_H
#define JSCOLLECTIONS_H

#include <jsScript/jsConstructor.h>
#include <jsScript/jsFunction.h>
#include <mofaTools/ref.h>

#include <list>

class jsConstructorCollection : public jsConstructor
{
public:
    jsConstructorCollection(jsTypeLibrary& typeLibrary);
    virtual ~jsConstructorCollection();
    virtual jsValue* construct(const Params& params);
    virtual void add(jsConstructor* constructor);
    void getTypeInfo(TypeInfoSet& members,int type,jsValue* parent=NULL);
    virtual const type_info& return_value_info();
    virtual std::string return_value();
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service);
    typedef std::list<mofa::ref<jsConstructor> > Constructors;
    const Constructors& GetConstructors() { return m_constructors; }
private:
    Constructors m_constructors;
};

class jsFunctionCollection : public jsFunction
{
public:
    jsFunctionCollection(jsTypeLibrary& typeLibrary);
    virtual ~jsFunctionCollection();
    virtual jsValue* call(const Params& params);
    virtual void add(jsFunction* function);
    void getTypeInfo(TypeInfoSet& members,int type,jsValue* parent=NULL);
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service);
    typedef std::list<mofa::ref<jsFunction> > Functions;
    const Functions& GetFunctions() { return m_functions; }
private:
    Functions m_functions;
};

#endif