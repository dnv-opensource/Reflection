//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#ifndef JSCONSTRUCTOR_H
#define JSCONSTRUCTOR_H

#include <jsScript/jsConversions.h>
#include <jsScript/jsMember.h>
#include "jsMemberLogger.h"
#include "jsInvoker.h"
#include "TypeUtilities/FunctionTraitsVariadic.h"

class JSSCRIPT_IMPORT_EXPORT jsConstructor : public jsMember
{
public:
    jsConstructor(jsTypeLibrary& typeLibrary,const std::string& member,bool isExplicit);
    jsConstructor(jsTypeLibrary& typeLibrary,bool isExplicit);
    virtual ~jsConstructor();
    virtual jsValue* call(const Params& params);
    virtual jsValue* construct(const Params& params) = 0;
    virtual std::string typeName();

    virtual jsValue* prototype();

    virtual int param_size();
    virtual std::string param_value(int i);
    virtual std::string return_value();
    virtual const type_info& return_value_info() = 0;
    virtual bool IsExplicit() const {return m_isExplicit;}
protected:
    virtual void jsConstructor::init();
    bool m_isExplicit;
};

template<class Fn>
class jsTConstructorFunction : public jsConstructor
{
public:
    typedef typename DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::ReturnType ReturnType;
    jsTConstructorFunction(jsTypeLibrary& typeLibrary,Fn f,const std::string& member,bool isExplicit=false) : jsConstructor(typeLibrary,member,isExplicit) , m_f(f) {init();}
    virtual ~jsTConstructorFunction() {}
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
    {
        jsMemberLogger(this, service, false);
    }
    virtual jsValue* construct(const std::vector<jsValue*>& params)
    {
        jsMemberLogger logger(this);
        return verify(jsInvoker::call(m_f,params));
    }
    virtual int param_size() 
    {         
        return DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Arity - (std::is_member_function_pointer_v<Fn> ? 1 : 0);
    }
    virtual std::string param_value(int i)
    {
        std::string result;
        DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Visit(ParamVisitor(result, GetTypeLibrary(), i + (std::is_member_function_pointer_v<Fn> ? 1 : 0)));
        return result;
    }
    virtual DNVS::MoFa::Reflection::Types::DecoratedTypeInfo GetParamType(int i) const
    {
        DNVS::MoFa::Reflection::Types::DecoratedTypeInfo result;
        DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Visit(ParamVisitorType(result, GetTypeLibrary(), i + (std::is_member_function_pointer_v<Fn> ? 1 : 0)));
        return result;
    }
    virtual bool IsVariadic() const {
        bool result = false;
        DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Visit(IsVariadicVisitor(result, GetTypeLibrary(), (int)DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Arity - 1));
        return result;

    }
    virtual size_t GetArity() const {
        return DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Arity;
    }

    virtual std::string return_value()
    {
        return GetTypeLibrary().GetTypeName(typeid(ReturnType));
    }
    virtual const type_info& return_value_info()
    {
        return typeid(ReturnType);
    }
private:
    Fn m_f;
};

template <class Signature>
class jsTConstructor : public jsConstructor
{
public:
    typedef typename DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::ReturnType ReturnType;
    jsTConstructor(jsTypeLibrary& typeLibrary,const std::string& member,bool isExplicit=false) : jsConstructor(typeLibrary,member,isExplicit) {init();}
    jsTConstructor(jsTypeLibrary& typeLibrary,bool isExplicit=false) : jsConstructor(typeLibrary,isExplicit) {}
    virtual ~jsTConstructor() {}
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
    {
        jsMemberLogger(this, service, false);
    }
    virtual jsValue* construct(const Params& params)
    {
        jsMemberLogger logger(this);        
        return verify(jsInvoker::construct(jsType<Signature>(),params));
    }
    virtual int param_size() 
    { 
        return DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::Arity;
    }
    virtual std::string param_value(int i)
    {
        std::string result;
        DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::Visit(ParamVisitor(result, GetTypeLibrary(), i));
        return result;
    }
    virtual DNVS::MoFa::Reflection::Types::DecoratedTypeInfo GetParamType(int i) const
    {
        DNVS::MoFa::Reflection::Types::DecoratedTypeInfo result;
        DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::Visit(ParamVisitorType(result, GetTypeLibrary(), i));
        return result;
    }
    virtual bool IsVariadic() const {
        bool result = false;
        DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::Visit(IsVariadicVisitor(result, GetTypeLibrary(), (int)DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::Arity - 1));
        return result;

    }
    virtual size_t GetArity() const {
        return DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Signature>::Arity;
    }
    virtual std::string return_value()
    {
        return GetTypeLibrary().GetTypeName(return_value_info());
    }
    virtual const type_info& return_value_info()
    {
        return typeid(ReturnType);
    }
};

#endif