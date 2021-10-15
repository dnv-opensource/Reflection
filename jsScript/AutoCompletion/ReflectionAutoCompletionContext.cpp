//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectionAutoCompletionContext.h"
#include "Reflection\Members\GlobalType.h"
#include "Reflection\Members\IMember.h"
#include "Reflection\Objects\Delegate.h"

namespace DNVS { namespace MoFa { namespace Scripting {

    ReflectionAutoCompletionContext::ReflectionAutoCompletionContext(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, const std::shared_ptr<IFunctionWrapper>& functionWrapper)
        : m_functionWrapper(functionWrapper)
        , m_typeLibrary(typeLibrary)
    {
        SetGlobalContext();
    }

    bool ReflectionAutoCompletionContext::IsGlobalContext() const
    {
        return m_context.IsConvertibleTo<Reflection::Members::GlobalType>();
    }
    
    void ReflectionAutoCompletionContext::CollectMembers(std::shared_ptr<jsAutoCompletion> autoComplete) const
    {
        CollectMembersTypeRecursive(autoComplete, m_context.GetType(), IsGlobalContext()? Reflection::Members::MemberType::TypeAll: Reflection::Members::MemberType::Value(~Reflection::Members::MemberType::TypeConstructor));
    }

    void ReflectionAutoCompletionContext::CollectFunctions(const Reflection::Objects::Object& function, std::shared_ptr<jsAutoCompletion> autoComplete) const
    {
        Reflection::Members::MemberPointer member;
        Reflection::Objects::Object object;
        std::string functionName;
        if (m_functionWrapper->TryUnwrapFunction(function, object, functionName))
        {
            if (object.GetType())
                member = object.GetType()->Lookup(functionName, DNVS::MoFa::Reflection::Variants::Variant());
        }
        else if (function.IsConvertibleTo<Reflection::Members::MemberPointer>())
            member = function.As<Reflection::Members::MemberPointer>();
        if (member)
        {
            for (auto overload : member->GetOverloads())
            {
                std::vector<std::string> arguments;
                bool isMember = (overload->GetMemberType()&DNVS::MoFa::Reflection::Members::MemberType::TypeMemberFunction) == DNVS::MoFa::Reflection::Members::MemberType::TypeMemberFunction;
                for (size_t i = 0; i < overload->GetArity(); ++i)
                {
                    if (i == 0 && isMember)
                    {
                        arguments.push_back("");
                    }
                    else
                    {
                        std::string arg = m_typeLibrary->GetTypeFormatter()->FormatType(overload->GetArgumentInfo(i)->GetTypeInfo());
                        if (!overload->GetArgumentInfo(i)->GetName().empty())
                            arg += " " + overload->GetArgumentInfo(i)->GetName();
                        arguments.push_back(arg);
                    }
                }
                std::string functionName = overload->ToString(arguments);
                if (*functionName.begin() == '.')
                    functionName = functionName.substr(1);
                functionName = m_typeLibrary->GetTypeFormatter()->FormatType(overload->GetReturnType()) + " " + functionName;
                autoComplete->addFunction(functionName);
            }
        }
    }
    bool ReflectionAutoCompletionContext::HasContext() const
    {
        return m_context.IsValid();
    }

    Reflection::Objects::Object ReflectionAutoCompletionContext::lookup(const std::string& function)
    {
        return m_functionWrapper->WrapFunction(m_context, function);
    }

    void ReflectionAutoCompletionContext::SetNestedContext(const Reflection::Objects::Object& nestedContext)
    {
        m_context = nestedContext.ConvertToDynamicType();
        if (m_typeLibrary->GetConversionGraph()->SupportsUnwrapping(m_context.GetDecoratedTypeInfo()))
            m_context.Reset(m_typeLibrary->GetConversionGraph()->TryUnwrapValue(m_context.GetVariant()));
    }
    
    void ReflectionAutoCompletionContext::SetGlobalContext()
    {
        m_context = Reflection::Objects::Object(m_typeLibrary, Reflection::Members::GlobalType());
    }

    void ReflectionAutoCompletionContext::CollectMembersType(std::shared_ptr<jsAutoCompletion> autoComplete, Reflection::TypeLibraries::TypePointer type, Reflection::Members::MemberType types) const
    {
        for (const std::string& memberName : type->GetUnsortedMembers())
        {
            auto member = type->Lookup(memberName);
            if (member->GetMemberType()&types)
            {
                autoComplete->addVariable(memberName);
            }
        }
    }

    void ReflectionAutoCompletionContext::CollectMembersTypeRecursive(std::shared_ptr<jsAutoCompletion> autoComplete, Reflection::TypeLibraries::TypePointer type, Reflection::Members::MemberType types) const
    {
        if (!type)
            return;
        CollectMembersType(autoComplete, type, types);
        for (auto baseType : type->GetBaseTypes())
        {
            CollectMembersTypeRecursive(autoComplete, m_typeLibrary->LookupType(baseType), types);
        }
    }

}}}