//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ScriptMember.h"
#include "..\jsTypeInfo.h"

namespace DNVS { namespace MoFa { namespace Doc {

    ScriptMember::ScriptMember(jsTypeInfo* info)
        : m_info(info)
    {
    }

    ScriptMember::MemberType ScriptMember::GetMemberType() const
    {
        switch (m_info->type())
        {
        case jsTypeInfo::jsFunction:
            return Function;
        case jsTypeInfo::jsConstructor:
            return Constructor;
        case jsTypeInfo::jsPropertyGet:
            return PropertyGet;
        case jsTypeInfo::jsPropertyPut:
            return PropertySet;
        case jsTypeInfo::jsPropertyPut|jsTypeInfo::jsPropertyGet:
            return Property;
        default:
            return Unknown;
        }
    }

    bool ScriptMember::HasArgument(size_t index) const
    {
        return m_info->argument(index) != nullptr;
    }

    std::string ScriptMember::GetArgumentTypeName(size_t index) const
    {
        auto argument = m_info->argument(index);
        if (argument)
            return argument->name();
        else
            return "";
    }

    std::string ScriptMember::GetReturnedTypeName() const
    {
        if (m_info->returned())
            return m_info->returned()->name();
        else
            return "";
    }

    std::string ScriptMember::GetReturnedDocumentation() const
    {
        if (m_info->returned())
            return m_info->returned()->docShort();
        else
            return "";
    }

    std::string ScriptMember::GetVariableName(size_t index, size_t varargIndex /*= 0*/) const
    {
        auto argument = m_info->argument(index);
        if (argument)
            return argument->varName(varargIndex);
        else
            return "";
    }

    std::string ScriptMember::GetArgumentDocumentation(size_t index, size_t varargIndex) const
    {
        auto argument = m_info->argument(index);
        if (argument)
            return argument->docShort(varargIndex);
        else
            return "";
    }

    std::string ScriptMember::GetName() const
    {
        return m_info->name();
    }

    std::string ScriptMember::GetDocumentation(bool full) const
    {
        return m_info->documentation(full ? jsTypeInfo::jsAutomatic : jsTypeInfo::jsShort);
    }

    std::vector<jsExampleInfo> ScriptMember::GetExamples() const
    {
        return m_info->getExamples();
    }

    std::string ScriptMember::ToString(bool sortable) const
    {
        return m_info->format(sortable);
    }

    std::string ScriptMember::GetNamespace() const
    {
        return m_info->nameSpace();
    }

    std::size_t ScriptMember::GetArity() const
    {
        return m_info->arguments();
    }

    std::size_t ScriptMember::GetNamedVarargCount(size_t index) const
    {
        auto argument = m_info->argument(index);
        if (argument)
            return argument->varNameCount();
        else
            return 0;
    }

}}}
