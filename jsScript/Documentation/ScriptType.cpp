//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ScriptType.h"
#include "..\jsValue.h"
#include "TypeMap.h"
#include "..\jsTypeInfo.h"

namespace DNVS { namespace MoFa { namespace Doc {

    ScriptType::ScriptType(jsValue* prototype, TypeMap& typeMap)
        : m_prototype(prototype)
        , m_typeMap(typeMap)
    {
    }

    std::string ScriptType::GetName() const
    {
        return m_prototype->typeName();
    }

    std::set<TypePointer> ScriptType::GetBaseClasses() const
    {
        if (m_prototype->prototype())
            return { m_typeMap.GetOrCreateFromPrototype(m_prototype->prototype()) };
        else
            return {};
    }

    std::set<TypePointer> ScriptType::GetDerivedClasses() const
    {
        std::set<TypePointer> derivedClasses;
        for (const auto& typePair : m_typeMap.GetPrototypeToTypeMap())
        {
            jsValue* value = typePair.first;
            if (value->prototype() == m_prototype)
                derivedClasses.insert(typePair.second);
        }
        return derivedClasses;
    }

    bool ScriptType::HasDocumentation() const
    {
        return m_prototype->typeInfo() != nullptr;
    }

    std::string ScriptType::GetDocumentation() const
    {
        if (HasDocumentation())
            return m_prototype->typeInfo()->documentation();
        else
            return "";
    }

    std::string ScriptType::GetExample() const
    {
        if (HasDocumentation())
            return m_prototype->typeInfo()->documentation(jsTypeInfo::jsExample);
        else
            return "";
    }

    std::vector<jsExampleInfo> ScriptType::GetExamples() const
    {
        if (HasDocumentation())
            return m_prototype->typeInfo()->getExamples();
        else
            return {};
    }

    std::vector<MemberPointer> ScriptType::GetMembers(IMember::MemberType memberType) const
    {
        std::vector<MemberPointer> members;
        jsValue::TypeInfoSet typeInfoSet;
        switch (memberType)
        {
        case IMember::Function:
            m_prototype->getTypeInfo(typeInfoSet, jsTypeInfo::jsFunction);
            break;
        case IMember::Constructor:
            m_prototype->getTypeInfo(typeInfoSet, jsTypeInfo::jsConstructor);
            break;
        case IMember::Property:
            m_prototype->getTypeInfo(typeInfoSet, jsTypeInfo::jsPropertyGet | jsTypeInfo::jsPropertyPut);
            break;
        case IMember::PropertyGet:
            m_prototype->getTypeInfo(typeInfoSet, jsTypeInfo::jsPropertyGet);
            break;
        case IMember::PropertySet:
            m_prototype->getTypeInfo(typeInfoSet, jsTypeInfo::jsPropertyPut);
            break;
        }
        for (auto method : typeInfoSet)
        {
            members.push_back(m_typeMap.CreateFromFunctionTypeInfo(method));
        }
        return members;
    }

    std::string ScriptType::ToString(bool sortable) const
    {
        if (m_prototype->typeInfo())
            return m_prototype->typeInfo()->format(sortable);
        else
            return GetName();
    }

    std::string ScriptType::GetNamespace() const
    {
        if (m_prototype->typeInfo())
            return m_prototype->typeInfo()->nameSpace();
        else
            return "";
    }

}}}


