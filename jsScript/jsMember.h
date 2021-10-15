//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMEMBER_H
#define JSMEMBER_H

#include <jsScript/jsValue.h>
#include <jsScript/jsTypeInfo.h>

#include <mofaTools/ref.h>
#include "Reflection/Types/DecoratedTypeInfo.h"

class jsScriptable;
class jsMemberInfo;
class jsTypeInfo;
class jsTypeLibrary;

/**
\brief Base class for all javascript functions, constructors and properties.
\example The expression "bm1=Beam(Point(0,0,0),Point(1,0,0));"
will first lookup the name "Beam" and find a constructor inheriting from jsMember.
It will then try to call this constructor with the correct input arguments to construct the beam.
*/
class JSSCRIPT_IMPORT_EXPORT jsMember : public jsValue
{
public:
    jsMember(jsTypeLibrary& typeLibrary, const std::string& member, int type = jsTypeInfo::jsUnknown);
    jsMember(jsTypeLibrary& typeLibrary);
    virtual ~jsMember();
    void registerObject(jsScriptable* object);
    jsValue* verify(jsValue* value);
    void getTypeInfo(TypeInfoSet& members, int type, jsValue* parent = NULL);
    jsTypeInfo* typeInfo();
    void SetTypeInfo(jsTypeInfo* info);
    jsTypeLibrary& GetTypeLibrary();
    const jsTypeLibrary& GetTypeLibrary() const;
    virtual std::string toString();
    ///For constructors, this function returns true if the constructor shall not be used for implicit argument conversions.
    virtual bool IsExplicit() const { return false; }
    virtual const type_info& class_type() const;
    virtual int param_size() { return 0; }
    virtual bool IsVariadic() const { return false; }
    virtual size_t GetArity() const { return 0; }
    virtual std::string param_value(int i) { return ""; }
    virtual DNVS::MoFa::Reflection::Types::DecoratedTypeInfo GetParamType(int i) const;
private:
    jsTypeLibrary& m_typeLibrary;
    mofa::ref<jsTypeInfo> m_typeInfo;
};

#endif
