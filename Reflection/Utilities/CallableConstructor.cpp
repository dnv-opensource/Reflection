//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CallableConstructor.h"
#include "Reflection/Attributes/DefaultConstructorSelectorAttribute.h"
#include "Reflection/Attributes/GenerateConstructorArgumentsAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/ListAsConstructorAttribute.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Members/MemberWithArguments.h"
#include "ConstructorSelector.h"
#include "Operators/Tags.h"
#include "Reflection/Objects/InvokeBinaryOperator.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    CallableConstructor::CallableConstructor(Members::MemberPointer member, const Objects::Object& object)
        : m_member(member)
        , m_object(object)
        , m_constructorArgumentsFromAttributes(false)
    {
        TypeUtilities::Tribool result = TryGenerateConstructorArgumentsFromAttribute();
        if (result == true)
            m_isValid = true;
        else if (result == false)
            m_isValid = false;
        else
            m_isValid = TryGenerateConstructorArgumentsFromProperties();
    }

    CallableConstructor::CallableConstructor()
        : m_isValid(false)
    {

    }

    TypeUtilities::Tribool CallableConstructor::TryGenerateConstructorArgumentsFromAttribute()
    {
        if (m_member->GetAttributeCollection().HasAttribute<Attributes::GenerateConstructorArgumentsAttribute>())
        {
            m_constructorArgumentsFromAttributes = true;
            return m_member->GetAttributeCollection().GetAttribute<Attributes::GenerateConstructorArgumentsAttribute>().GenerateConstructorArguments(m_object, m_arguments);
        }
        //Special deduction for members with arity 0
        else if (
            (
                m_member->GetArity() == 0 || 
                (
                    m_member->GetMinimumArgumentCount() == 0 && 
                    m_member->GetMemberType() == Members::MemberType::TypePropertyGet
                )
            ) && m_object.GetTypeLibrary())
        {
            try {
                auto member = Objects::PrepareInvokeBinaryOperator<DNVS::MoFa::Operators::Tags::EqualTo>(m_object.GetTypeLibrary(), m_object.GetVariant(), m_member->Invoke({}));
                if (member && member->IsOk())
                {
                    Objects::Object result(m_object.GetTypeLibrary(), member->Invoke());
                    if (result.IsConvertibleTo<bool>())
                    {
                        m_constructorArgumentsFromAttributes = true;
                        return result.As<bool>();
                    }
                }
            }
            catch(...){}
        }
        return TypeUtilities::Indeterminate;
    }
    TypeUtilities::Tribool CallableConstructor::TryGenerateConstructorArgumentsFromProperties()
    {
        if (!m_object.GetTypeLibrary())
            return false;
        TypeLibraries::TypePointer type = m_object.GetTypeLibrary()->LookupType(m_member->GetReturnType().GetTypeInfo());
        if (!type)
            return false;
        m_arguments.resize(m_member->GetArity());
        try {
            for (size_t i = 0; i < m_member->GetArity(); ++i)
            {
                std::string name = m_member->GetArgumentInfo(i)->GetName();
                auto member = type->Lookup(name, Variants::Variant());
                if (!member)
                    return false;
                for (auto overload : member->GetOverloads())
                {
                    if (overload->GetMemberType() == Members::MemberType::TypePropertyGet)
                    {
                        Members::MemberWithArguments callableOverload(m_object.GetTypeLibrary()->GetConversionGraph(), overload, { m_object.GetVariant() });
                        if (!callableOverload.IsOk())
                            return false;
                        m_arguments[i] = callableOverload.Invoke();
                    }
                }
                if (!m_arguments[i].IsValid())
                    return false;
            }
        }
        catch (...)
        {
            return false;
        }
        if (m_member->GetArity() == 0)
        {
            return m_object.IsConvertibleTo(m_member->GetReturnType());
        }
        else
            return true;
    }

    bool CallableConstructor::IsDefaultConstructor() const
    {
        if (m_member->GetAttributeCollection().HasAttribute<Attributes::DefaultConstructorSelectorAttribute>())
            return m_member->GetAttributeCollection().GetAttribute<Attributes::DefaultConstructorSelectorAttribute>().IsDefaultConstructor(m_object);
        else
            return true;
    }

    bool CallableConstructor::IsValid() const
    {
        return m_isValid;
    }

    bool CallableConstructor::IsInfinitlyRecursive() const
    {
        for (size_t i = 0; i < m_arguments.size(); ++i)
        {
            if (m_member->GetArgumentInfo(i)->GetTypeInfo().GetTypeInfo() == m_object.GetDecoratedTypeInfo().GetTypeInfo())
            {
                auto constructor = ConstructorSelector(Objects::Object(m_object.GetTypeLibrary(), m_arguments[i]), false).GetBestConstructor();
                if (constructor.GetMember() == m_member)
                    return true;
            }
        }
        return false;
    }

    bool CallableConstructor::operator<(const CallableConstructor& other) const
    {
        if (IsDefaultConstructor() != other.IsDefaultConstructor())
            return IsDefaultConstructor();
        //Prioritize constructors that has the GenerateConstructorArgumentsAttribute
        if (HasGenerateConstructorArgumentsAttribute() != other.HasGenerateConstructorArgumentsAttribute())
            return HasGenerateConstructorArgumentsAttribute();

        else if (HasMemberArgumentOfSameTypeAsThis() != other.HasMemberArgumentOfSameTypeAsThis())
            return !HasMemberArgumentOfSameTypeAsThis();
        //Prioritize constructors that has the ListAsConstructorAttribute
        if (HasListAsConstructorAttribute() != other.HasListAsConstructorAttribute())
            return HasListAsConstructorAttribute();
        return m_member < other.m_member;
    }

    const Members::MemberPointer& CallableConstructor::GetMember() const
    {
        return m_member;
    }

    const std::vector<Variants::Variant>& CallableConstructor::GetArguments() const
    {
        return m_arguments;
    }

    bool CallableConstructor::HasGenerateConstructorArgumentsAttribute() const
    {
        return m_constructorArgumentsFromAttributes;
    }

    bool CallableConstructor::HasListAsConstructorAttribute() const
    {
        return m_member->GetAttributeCollection().HasAttribute<Attributes::ListAsConstructorAttribute>();
    }

    bool CallableConstructor::HasMemberArgumentOfSameTypeAsThis() const
    {
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            if (m_member->GetArgumentInfo(i)->GetTypeInfo().GetTypeInfo() == m_object.GetDecoratedTypeInfo().GetTypeInfo())
                return true;
        }
        return false;
    }

}}}}