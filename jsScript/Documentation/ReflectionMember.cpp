//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionMember.h"
#include "TypeMap.h"
#include "IType.h"
#include "Reflection\Attributes\AttributeCollection.h"
#include "Reflection\Attributes\DocumentationAttribute.h"
#include "..\jsTypeInfo.h"
#include "..\jsStack.h"

namespace DNVS { namespace MoFa { namespace Doc {
    using namespace Reflection::Attributes;

    ReflectionMember::ReflectionMember(const Reflection::Members::MemberPointer& member, TypeMap& typeMap)
        : m_member(member)
        , m_typeMap(typeMap)
    {

    }

    DNVS::MoFa::Doc::IMember::MemberType ReflectionMember::GetMemberType() const
    {
        return ConvertMemberType(m_member->GetMemberType());
    }

    IMember::MemberType ReflectionMember::ConvertMemberType(DNVS::MoFa::Reflection::Members::MemberType memberType)
    {
        switch (memberType)
        {
        case Reflection::Members::MemberType::TypeConstructor:
            return IMember::Constructor;
        case Reflection::Members::MemberType::TypeFunction:
        case Reflection::Members::MemberType::TypeMemberFunction:
        case Reflection::Members::MemberType::TypeStaticFunction:
            return IMember::Function;
        case Reflection::Members::MemberType::TypePropertyGet:
            return IMember::PropertyGet;
        case Reflection::Members::MemberType::TypePropertySet:
            return IMember::PropertySet;
        case Reflection::Members::MemberType::TypePropertyGetSet:
            return IMember::Property;
        case Reflection::Members::MemberType::TypeOperator:
            return IMember::Operator;
        default:
            return IMember::Unknown;
        }
    }

    bool ReflectionMember::IsMember() const
    {
        switch (m_member->GetMemberType())
        {
        case Reflection::Members::MemberType::TypeMemberFunction:
            return true;
        default:
            return false;
        }
    }

    bool ReflectionMember::IsProperty() const
    {
        switch (m_member->GetMemberType())
        {
        case Reflection::Members::MemberType::TypePropertyGet:
        case Reflection::Members::MemberType::TypePropertySet:
        case Reflection::Members::MemberType::TypePropertyGetSet:
            return true;
        default:
            return false;
        }
    }

    bool ReflectionMember::HasArgument(size_t index) const
    {
        index = GetModifiedArgumentIndex(index);
        return index < m_member->GetMaximumArgumentCount();
            
    }

    std::string ReflectionMember::GetArgumentTypeName(size_t index) const
    {
        index = GetModifiedArgumentIndex(index);
        auto type = m_typeMap.GetOrCreateFromTypeIndex(m_member->GetArgumentInfo(index)->GetTypeInfo().GetTypeInfo());
        if (type && !type->GetName().empty())
            return type->GetName();
        else
            return jsStack::stack()->typeName(m_member->GetArgumentInfo(index)->GetTypeInfo().GetTypeInfo().name());
    }

    std::string ReflectionMember::GetReturnedTypeName() const
    {
        if (IsProperty() && m_member->GetMemberType() == Reflection::Members::MemberType::TypePropertySet)
        {
            return GetArgumentTypeName(0);
        }            
        auto type = m_typeMap.GetOrCreateFromTypeIndex(m_member->GetReturnType().GetTypeInfo());
        if (type && !type->GetName().empty())
            return type->GetName();
        else
        {
            return jsStack::stack()->typeName(m_member->GetReturnType().GetTypeInfo().name());
        }
    }

    std::string ReflectionMember::GetReturnedDocumentation() const
    {
        return "";
    }

    std::string ReflectionMember::GetVariableName(size_t index, size_t varargIndex /*= 0*/) const
    {
        index = GetModifiedArgumentIndex(index);
        return m_member->GetArgumentInfo(index)->GetName();
    }

    std::string ReflectionMember::GetArgumentDocumentation(size_t index, size_t varargIndex /*= 0*/) const
    {
        index = GetModifiedArgumentIndex(index);
        return m_member->GetArgumentInfo(index)->GetDocumentation();
    }

    std::size_t ReflectionMember::GetNamedVarargCount(size_t index) const
    {
        index = GetModifiedArgumentIndex(index);
        if (!m_member->GetArgumentInfo(index)->GetName().empty())
            return 1;
        else
            return 0;
    }

    std::size_t ReflectionMember::GetArity() const
    {
        if (IsMember())
            return m_member->GetArity() - 1;
        else if (IsProperty())
            return 0;
        else
            return m_member->GetArity();
    }

    std::string ReflectionMember::GetName() const
    {
        if (m_member->GetName() == DNVS::MoFa::Reflection::Members::ConstructorString)
            return GetReturnedTypeName();
        else
            return m_member->GetName();
    }

    std::string ReflectionMember::GetDocumentation(bool full /*= true*/) const
    {
        if (m_member->GetAttributeCollection().HasAttribute<DocumentationAttribute>())
        {
            std::string doc = m_member->GetAttributeCollection().GetAttribute<DocumentationAttribute>().GetDocumentation();
            if (!full)
                return doc.substr(0, doc.find_first_of("\n"));
            else
                return doc;
        }
        return "";
    }

    std::vector<jsExampleInfo> ReflectionMember::GetExamples() const
    {
        return {};
    }

    std::string ReflectionMember::ToString(bool sortable) const
    {
        if (sortable)
            return "z::" + GetName();

        std::vector<std::string> arguments;
        for (size_t i = 0; i < GetArity(); ++i)
        {
            arguments.push_back(GetArgumentTypeName(i));
        }
        return m_member->ToString(arguments);
    }

    std::string ReflectionMember::GetNamespace() const
    {
        return "";
    }

    size_t ReflectionMember::GetModifiedArgumentIndex(size_t index) const
    {
        if (IsMember() || IsProperty())
            return index + 1;
        else
            return index;
    }

}}}
