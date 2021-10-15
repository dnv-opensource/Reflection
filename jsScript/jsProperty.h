//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSPROPERTY_H
#define JSPROPERTY_H

#include <jsScript/jsConversions.h>
#include <jsScript/jsMember.h>
#include <jsScript/properties.hpp>
#include "ClassTypeGetter.h"
#include <jsScript/jsTypeInfo.h>
#include "TypeUtilities/FunctionTraits.h"
#include "jsInvoker.h"

class JSSCRIPT_IMPORT_EXPORT jsPropertyGet : public jsMember
{
public:
    jsPropertyGet(jsTypeLibrary& typeLibrary) : jsMember(typeLibrary,"",jsTypeInfo::jsPropertyGet) {}
    virtual ~jsPropertyGet() {}
    jsValue* duplicate(jsValue* owner) = 0;
    virtual std::string propertyType() const = 0;
    virtual bool IsConst() const = 0;
};

class JSSCRIPT_IMPORT_EXPORT jsPropertyPut : public jsMember
{
public:
    jsPropertyPut(jsTypeLibrary& typeLibrary) : jsMember(typeLibrary, "", jsTypeInfo::jsPropertyPut) {}
    virtual ~jsPropertyPut() {}
    jsValue* assign(jsValue* owner,jsValue* op2) = 0;
    virtual std::string propertyType() const = 0;
};

class JSSCRIPT_IMPORT_EXPORT jsProperty : public jsMember
{
public:
    jsProperty(jsTypeLibrary& typeLibrary,const std::string& member) : jsMember(typeLibrary,member) , m_get(NULL) , m_put(NULL) {}
    virtual ~jsProperty() {}
    jsValue* setGet(jsPropertyGet* get);
    jsValue* setPut(jsPropertyPut* put);
    jsValue* lookup(const std::string& identifier,jsValue* owner=NULL);
    jsValue* duplicate(jsValue* owner);
    jsValue* assign(jsValue* owner,jsValue* op2);
    jsValue* call(const Params& params);
    virtual std::string typeName();
    jsPropertyGet* GetGetter() const {return m_get;}
    jsPropertyPut* GetSetter() const {return m_put;}
    virtual const type_info& class_type() const
    {
        if(m_get)
            return m_get->class_type();
        if(m_put)
            return m_put->class_type();
        return typeid(void);
    }
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service);
private:
    mofa::ref<jsPropertyGet> m_get;
    mofa::ref<jsPropertyPut> m_put;
};

template<typename Signature>
struct IsPropertyGetConst : std::false_type 
{};

template<typename R,typename Cls>
struct IsPropertyGetConst<R(Cls::*)() const> : std::true_type
{};

template<typename T>
struct PropertyReturnType
{
    static const type_info& GetType() { return typeid(T); }
};

template<typename R, typename ClassT>
struct PropertyReturnType<R(ClassT::*)() const>
{
    static const type_info& GetType() { return typeid(R); }
};

template<typename R, typename ClassT>
struct PropertyReturnType<R(ClassT::*)()>
{
    static const type_info& GetType() { return typeid(R); }
};

template<typename R>
struct PropertyReturnType<R(*)()>
{
    static const type_info& GetType() { return typeid(R); }
};

template<class Get>
class jsTPropertyGet : public jsPropertyGet
{
public:
    jsTPropertyGet(jsTypeLibrary& typeLibrary,Get get) : jsPropertyGet(typeLibrary), m_get(get) {}
    virtual ~jsTPropertyGet() {}
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
    {
        jsMemberLogger(this, service, false);
    }
    virtual jsValue* duplicate(jsValue* owner) 
    {
        jsMemberLogger logger(this);
        return verify(getter::duplicate(m_get,owner));
    }
    std::string propertyType() const {
        return GetTypeLibrary().GetTypeName(PropertyReturnType<Get>::GetType());
    }
    virtual bool IsConst() const {
        return IsPropertyGetConst<Get>::value;
    }
    virtual const type_info& class_type() const
    {
        return ClassTypeGetter<DNVS::MoFa::TypeUtilities::IsMember<Get>::value>::Get(m_get);
    }
private:
    Get m_get;
};

template<typename T>
struct PropertyArgumentType
{
    static const type_info& GetType() { return typeid(T); }
};

template<typename R, typename ClassT,typename Arg>
struct PropertyArgumentType<R(ClassT::*)(Arg)>
{
    static const type_info& GetType() { return typeid(Arg); }
};

template<class Put>
class jsTPropertyPut : public jsPropertyPut
{
public:
    jsTPropertyPut(jsTypeLibrary& typeLibrary,Put put) : jsPropertyPut(typeLibrary), m_put(put) {}
    virtual ~jsTPropertyPut() {}
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service)
    {
        jsMemberLogger(this, service, false);
    }
    jsValue* assign(jsValue* owner,jsValue* op2) {
        jsMemberLogger logger(this);
        return setter::assign(m_put,owner,op2);
    }
    std::string propertyType() const {
        return GetTypeLibrary().GetTypeName(PropertyArgumentType<Put>::GetType());
    }   

    virtual const type_info& class_type() const
    {
        return ClassTypeGetter<DNVS::MoFa::TypeUtilities::IsMember<Put>::value>::Get(m_put);
    }
    virtual int param_size() {return 1;}
    virtual std::string param_value(int i)
    {
        return propertyType();
    }
private:
    template<class A1>
    std::string propertyType(A1) const {
        typedef typename A1::type argument_type;
        return jsStack::stack()->typeName(typeid(argument_type));
    }
    Put m_put;
};

#endif