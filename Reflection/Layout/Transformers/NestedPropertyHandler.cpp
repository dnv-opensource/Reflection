//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "NestedPropertyHandler.h"
#include "boost/algorithm/string/split.hpp"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    NestedProperty::NestedProperty(const TypeLibraries::TypeLibraryPointer& typeLibrary, const TypeLibraries::TypePointer& type, const std::string& name)
        : m_parentType(type)
        , m_typeLibrary(typeLibrary)
    {
        Members::MemberPointer member = m_parentType->Lookup(name, Variants::Variant());
        if(member)
        {
            for(const Members::MemberPointer& overload : member->GetOverloads())
            {
                if (overload->GetMemberType() == Members::MemberType::TypePropertyGet)
                {
                    m_context.getter = overload;
                    m_context.parentType = overload->GetArgumentInfo(0)->GetTypeInfo();
                    m_context.type = overload->GetReturnType();
                }
                else if (overload->GetMemberType() == Members::MemberType::TypePropertySet)
                {
                    m_context.setter = overload;
                    m_context.parentType = overload->GetArgumentInfo(0)->GetTypeInfo();
                    m_context.type = overload->GetArgumentInfo(1)->GetTypeInfo();
                }
            }
        }
    }

    NestedProperty* NestedProperty::Add(const std::string& name)
    {
        auto it = m_nestedProperties.find(name);
        if(it == m_nestedProperties.end())
        {
            auto type = m_typeLibrary->LookupType(m_context.type.GetTypeInfo());
            if (type)
                it = m_nestedProperties.emplace(name, NestedProperty(m_typeLibrary, type, name)).first;
            else
                return nullptr;
        }
        return &it->second;
    }

    const NestedProperty* NestedProperty::Lookup(const std::string& name) const
    {
        auto it = m_nestedProperties.find(name);
        if (it == m_nestedProperties.end())
        {
            return nullptr;
        }
        return &it->second;
    }

    const LayoutContext& NestedProperty::GetContext() const
    {
        return m_context;
    }

    void NestedProperty::Finalize()
    {
        for (auto& nested : m_nestedProperties)
            nested.second.Finalize();
        if(!m_nestedProperties.empty())
        {
            auto constructors = Utilities::ConstructorsFromTypeService::GetService(m_typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(m_context.type, false);
            for (const auto& constructor : constructors)
            {
                if (IsValidConstructor(constructor))
                {
                    m_context.function = constructor;
                    break;
                }
            }
        }
    }

    bool NestedProperty::IsValidConstructor(const Members::MemberPointer& member) const
    {
        if (m_nestedProperties.size() < member->GetMinimumArgumentCount())
            return false;
        if (m_nestedProperties.size() > member->GetMaximumArgumentCount())
            return false;
        size_t argumentsFound = 0;
        for (size_t i = 0; i < member->GetArity(); ++i)
        {
            if (m_nestedProperties.count(member->GetArgumentInfo(i)->GetName()) > 0)
                ++argumentsFound;
            else if (!member->GetArgumentInfo(i)->HasDefault())
                return false;
        }
        return argumentsFound == m_nestedProperties.size();
    }

    NestedPropertyHandler::NestedPropertyHandler(const TypeLibraries::TypeLibraryPointer& typeLibrary, const TypeLibraries::TypePointer& type)
        : m_typeLibrary(typeLibrary)
        , m_type(type)
    {

    }

    NestedProperty* NestedPropertyHandler::CreateProperty(const std::string& name)
    {
        std::vector<std::string> nested;
        boost::algorithm::split(nested, name, [](char c) {return c == '.'; });
        NestedProperty* current = nullptr;
        for(size_t i = 0; i < nested.size(); ++i)
        {
            if (!current)
            {
                auto it = m_nestedProperties.find(nested[i]);
                if (it == m_nestedProperties.end())
                    it = m_nestedProperties.emplace(nested[i], NestedProperty(m_typeLibrary, m_type, nested[i])).first;
                current = &it->second;
            }
            else
            {
                current = current->Add(nested[i]);
                if (!current)
                    return nullptr;
            }
        }
        return current;
    }

    const NestedProperty* NestedPropertyHandler::LookupProperty(const std::string& name) const
    {
        std::vector<std::string> nested;
        boost::algorithm::split(nested, name, [](char c) {return c == '.'; });
        const NestedProperty* current = nullptr;
        for (size_t i = 0; i < nested.size(); ++i)
        {
            if (!current)
            {
                auto it = m_nestedProperties.find(nested[i]);
                if (it == m_nestedProperties.end())
                    return nullptr;
                current = &it->second;
            }
            else
            {
                current = current->Lookup(nested[i]);
                if (!current)
                    return nullptr;
            }
        }
        return current;
    }

    void NestedPropertyHandler::Finalize()
    {
        for (auto& nested : m_nestedProperties)
            nested.second.Finalize();
    }

}}}}

