//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFUNCTION_H
#define JSFUNCTION_H

#include <jsScript/jsConversions.h>
#include <jsScript/jsMember.h>
#include "jsInvoker.h"
#include "ClassTypeGetter.h"
#include "jsMemberLogger.h"

#include "Reflection/TypeConversions/InheritanceConversions.h"
#include "TypeUtilities/FunctionTraitsVariadic.h"

class JSSCRIPT_IMPORT_EXPORT jsFunction : public jsMember
{
public:
    jsFunction(jsTypeLibrary& typeLibrary,const std::string& member);
    jsFunction(jsTypeLibrary& typeLibrary);
    virtual ~jsFunction();
    virtual jsValue* call(const std::vector<jsValue*>& params) = 0;
    virtual std::string typeName();
    virtual int param_size();
    virtual std::string param_value(int i);
    virtual std::string return_value();
protected:
    virtual void init();
};

template<class Fn>
class jsTFunction : public jsFunction
{
public:
    typedef typename DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::ReturnType ReturnType;
    jsTFunction(jsTypeLibrary& typeLibrary, Fn f, const std::string& member) : jsFunction(typeLibrary, member), m_f(f)
    {
        init();

        DNVS::MoFa::Reflection::TypeConversions::AddInheritanceConversions<jsValue, jsTFunction<Fn>>(
            jsStack::stack()->GetConversionGraph());
    }

    virtual ~jsTFunction()
    {
    }

    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
    {
        jsMemberLogger(this, service, false);
    }
    virtual jsValue* call(const std::vector<jsValue*>& params)
    {
        jsMemberLogger logger(this);
        return verify(jsInvoker::call(m_f,params));
    }
    virtual int param_size()
    {
        return DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Arity - (std::is_member_function_pointer_v<Fn> ? 1 : 0);
    }
    virtual size_t GetArity() const { 
        return DNVS::MoFa::TypeUtilities::FunctionTraitsVariadic<Fn>::Arity;
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
    virtual std::string return_value()
    {
        return GetTypeLibrary().GetTypeName(typeid(ReturnType));
    }
    virtual const type_info& class_type() const
    {
        return ClassTypeGetter<DNVS::MoFa::TypeUtilities::IsMember<Fn>::value>::Get(m_f);
    }
private:
    Fn m_f;
};

inline jsValue* invoke(jsFunction* function) {
    std::vector<jsValue*> params;
    return function->call(params);
}

template<typename A1>
jsValue* invoke(jsFunction* function,const A1& a1) {
    std::vector<jsValue*> params;
    params.push_back(toJScript(a1));
    return function->call(params);
}

template<typename A1,typename A2>
jsValue* invoke(jsFunction* function,const A1& a1,const A2& a2) {
    std::vector<jsValue*> params;
    params.push_back(toJScript(a1));
    params.push_back(toJScript(a2));
    return function->call(params);
}

template<typename A1,typename A2,typename A3>
jsValue* invoke(jsFunction* function,const A1& a1,const A2& a2,const A3& a3) {
    std::vector<jsValue*> params;
    params.push_back(toJScript(a1));
    params.push_back(toJScript(a2));
    params.push_back(toJScript(a3));
    return function->call(params);
}

template<typename A1,typename A2,typename A3,typename A4>
jsValue* invoke(jsFunction* function,const A1& a1,const A2& a2,const A3& a3,const A4& a4) {
    std::vector<jsValue*> params;
    params.push_back(toJScript(a1));
    params.push_back(toJScript(a2));
    params.push_back(toJScript(a3));
    params.push_back(toJScript(a4));
    return function->call(params);
}

template<typename A1,typename A2,typename A3,typename A4,typename A5>
jsValue* invoke(jsFunction* function,const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5) {
    std::vector<jsValue*> params;
    params.push_back(toJScript(a1));
    params.push_back(toJScript(a2));
    params.push_back(toJScript(a3));
    params.push_back(toJScript(a4));
    params.push_back(toJScript(a5));
    return function->call(params);
}

#endif
