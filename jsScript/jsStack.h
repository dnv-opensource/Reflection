//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSSTACK_H
#define JSSTACK_H

#include <jsScript/jsScript_config.h>
#include <jsScript/jsStackElement.h>
#include <stack>
#include "Reflection/TypeConversions/IConversionGraph.h"


namespace DNVS { namespace MoFa {namespace Reflection {namespace TypeLibraries {
    class ITypeLibrary;
    typedef std::shared_ptr<ITypeLibrary> TypeLibraryPointer;
}}}}

class jsUnit;
class jsUnitValue;
class jsTypeLibrary;

/**
\brief Defines the javascript variable stack.
This class will manage the lifetime of temporary stack objects created during e.g. javascript function invocations.
*/
class JSSCRIPT_IMPORT_EXPORT jsStack
{
public:
    jsStack(jsTypeLibrary& typeLibrary);
    virtual ~jsStack();

    ///Try to implicitly construct an object of type based on the input parameter.
    virtual jsValue* construct(const type_info& type, jsValue* param);

    virtual bool validUnit(const std::string& expression);
    virtual jsUnit* createUnit(const std::string& expression);
    virtual jsUnitValue* createUnitValue(double value, jsUnit* unit);

    ///Stack functionality
    jsValue* insert(jsValue* value);
    std::string& insert(const std::string& value);
    void insert(const DNVS::MoFa::Reflection::Variants::Variant& var);
    void push();
    void pop();
    static bool HasStack();
    static jsStack* stack();
    static void setStack(jsStack* stack);
    bool isBaseAndDerived(const type_info& base, const type_info& derived);
    ///Dummy object functionality
    virtual void addDummy(const type_info& info, jsValue* value);
    virtual jsValue* getDummy(const type_info& info, const type_info& owner);
    virtual jsValue* getDummy(const type_info& info);
    virtual jsValue* getDummy(const std::string& info);
    virtual bool dummyMode();
    virtual void setDummyMode(bool dummyMode);

    ///TypeName functionality
    std::string typeName(const type_info& info);
    std::string typeName(const std::string& info);
    void addType(const type_info& info, const std::string& name);
    void SetTypeLibrary(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, bool registerIntegerDivide = true);
    DNVS::MoFa::Reflection::TypeConversions::ConversionGraphPointer GetConversionGraph();
    DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer GetTypeLibrary();
    jsTypeLibrary& GetJsTypeLibrary();
private:
    jsTypeLibrary& m_jstypeLibrary;
    std::stack<jsStackElement> m_elements;
    static jsStack* m_stack;

    DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer m_typeLibrary;
    typedef std::map<std::string, mofa::ref<jsValue> > DummyObjects;
    DummyObjects m_dummyObjects;
    bool m_dummyMode;

    int m_constructionStack;
};

class JSSCRIPT_IMPORT_EXPORT jsStackPusher {
public:
    jsStackPusher();
    ~jsStackPusher();
};

#endif