//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSVTBL_H
#define JSVTBL_H


#pragma warning(disable:4786)
#include <jsScript/jsScript_config.h>
#include <string>
#include <map>
#include <set>

#include <mofatools/ref.h>

#include <jsScript/jsValue.h>
#include "jsTypeInfo.h"

class jsConstructor;
class jsFunction;
class jsTypeLibrary;

class vtbl_not_found : public std::exception {
public:
    vtbl_not_found(const type_info& info);
private:
    static std::string errorMessage(const type_info& info);
};

class JSSCRIPT_IMPORT_EXPORT jsVTBL : public jsValue
{
public:
    jsVTBL(const type_info& info,jsTypeLibrary& typeLibrary,jsVTBL* parent=NULL);
    virtual ~jsVTBL();
    jsValue* add(const std::string& identifier,jsValue* value);
    jsValue* lookup(const std::string& identifier,jsValue* owner=NULL);
    bool methodName(jsValue* value,std::string& identifier);
    void unite(jsVTBL* vtbl2);
    bool isParent(jsVTBL* vtbl);

    bool isEnabled() const;
    void enable(bool flag);

    virtual jsValue* prototype();

    void getTypeInfo(TypeInfoSet& members,int type,jsValue* parent=NULL);
    virtual jsTypeInfo* typeInfo();
	const type_info& GetTypeInformation() const;

    typedef std::map<std::string,mofa::ref<jsValue> > VTBL;
    typedef VTBL::iterator vtbl_iterator;
    vtbl_iterator vtbl_begin();
    vtbl_iterator vtbl_end();
    size_t vtbl_size();

    virtual jsValueIterator child_begin() const;
    virtual jsValueIterator child_end() const;
    virtual size_t          child_size() const;

    typedef std::vector<jsVTBL*> Union;
    typedef Union::iterator union_iterator;
    union_iterator union_begin();
    union_iterator union_end();
    size_t union_size();
    virtual std::string typeName();
    void LogFunctionDetails(const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service);
    jsTypeLibrary& GetTypeLibrary() { return m_typeLibrary; }
private:
    jsValue* addConstructorCollection(const std::string& identifier,mofa::ref<jsConstructor> c1,mofa::ref<jsConstructor> c2);
    jsValue* addFunctionCollection(const std::string& identifier,mofa::ref<jsFunction> f1,mofa::ref<jsFunction> f2);
    void getUnions(Union& vtbls);
	const std::type_info& m_typeInformation;
    std::string m_typeName;
    jsVTBL* m_parent;
    jsTypeLibrary& m_typeLibrary;
    Union m_union;

    VTBL m_vtbl;
    VTBL m_caseless_vtbl;
    mofa::ref<jsTypeInfo> m_typeInfo;
    bool m_enabled;
};

#endif
