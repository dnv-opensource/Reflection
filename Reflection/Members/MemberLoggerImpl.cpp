//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberLoggerImpl.h"
#include "Services/Module.h"
#include "MemberLoggerContext.h"
#include "boost/algorithm/string/replace.hpp"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
MemberLoggerImpl::MemberLoggerImpl(IMember* member, bool logCall)
    : m_member(member)
    , m_start(clock())
    , m_logCall(logCall)
{
    Initialize();
}

MemberLoggerImpl::MemberLoggerImpl(IMember* member, const std::shared_ptr<Members::MemberLoggerContext>& context, bool logCall /*= true*/)
    : m_member(member)
    , m_start(clock())
    , m_logCall(logCall)
{
    m_member->SetMemberLoggerContext(context);
    Initialize();
}

void MemberLoggerImpl::Initialize()
{
    MemberLoggerContext* context = m_member->GetMemberLoggerContext().get();
    if (context == nullptr)
        return;

    using namespace DNVS::MoFa::Services;
    m_dll = Module(Detail::ModuleHandle(typeid(*m_member))).FileName();

    switch (m_member->GetMemberType())
    {
    case MemberType::TypeConstructor:
        m_function = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
        if (context)
        {
            m_class = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
            m_signature = CreateConstructorSignature(context);
        }
        break;
    case MemberType::TypeMemberFunction:
        m_function = m_member->GetName();
        if (context)
        {
            m_class = context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(0)->GetTypeInfo());
            m_signature = CreateMemberFunctionSignature(context);
        }
        break;
    case MemberType::TypeStaticFunction:
        m_function = m_member->GetName();
        if (context)
        {
            m_class = "void"; // Does not have access to class name
            m_signature = CreateStaticFunctionSignature(context);
        }
        break;
    case MemberType::TypeFunction:
        m_function = m_member->GetName();
        if (context)
        {
            m_class = "void"; // Does not have access to class name
            m_signature = CreateFunctionSignature(context);
        }
        break;
    case MemberType::TypePropertyGet:
        m_function = m_member->GetName();
        if (context)
        {
            m_class = context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(0)->GetTypeInfo());
            m_signature = CreatePropertyGetSignature(context);
        }
        break;
    case MemberType::TypePropertySet:
        m_function = m_member->GetName();
        if (context)
        {
            m_class = context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(0)->GetTypeInfo());
            m_signature = CreatePropertySetSignature(context);
        }
        break;
    case MemberType::TypeOperator:
        {
            std::vector<std::string> arguments(m_member->GetArity());
            m_function = m_member->ToString(arguments);
            if (context)
            {
                m_class = context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(0)->GetTypeInfo());
                m_signature = CreateOperatorSignature(context);
            }
        }
        break;
    default:
        {
            std::string typeStr = m_member->GetMemberType().ToString();
            m_function = "Unknown type: " + m_member->GetName() + " - scripting command cannot be decided.";
            if (context)
            {
                m_class = "";
                m_signature = "";
            }
        }
        break;
    }

    if (m_member->GetMemberLoggerContext()->GetFunctionDetailService())
    {
        std::lock_guard<std::mutex> guard(m_member->GetMemberLoggerContext()->GetFunctionDetailService()->GetMutex());
        auto function = m_member->GetMemberLoggerContext()->GetFunctionDetailService()->GetOrCreateFunctionDetails(m_dll, m_class, m_function, m_signature);
        if (function && m_logCall)
            function->AddCall(double(clock() - m_start) / CLOCKS_PER_SEC);
    }
}

MemberLoggerImpl::~MemberLoggerImpl()
{
}

std::string MemberLoggerImpl::CreateMemberFunctionSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("");
    std::string returnType = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
    if (returnType.size() > 0)
    {
        signature += returnType + "(";
    }
    else
    {
        signature += "void(";
    }

    for (int i = 1; i < m_member->GetArity(); ++i)
    {
        if (i > 1) signature += ", ";
        signature += context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) + 
            " " + m_member->GetArgumentInfo(i)->GetName();
    }
    signature += ")";
    return signature;
}

std::string MemberLoggerImpl::CreateStaticFunctionSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("");
    std::string returnType = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
    if (returnType.size() > 0)
    {
        signature += returnType + "(";
    }
    else
    {
        signature += "void(";
    }

    for (int i = 0; i < m_member->GetArity(); ++i)
    {
        if (i > 0) signature += ", ";
        signature += context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) +
            " " + m_member->GetArgumentInfo(i)->GetName();
    }
    signature += ")";
    return signature;
}

std::string MemberLoggerImpl::CreateFunctionSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("");
    std::string returnType = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
    if (returnType.size() > 0)
    {
        signature += returnType + "(";
    }
    else
    {
        signature += "void(";
    }

    for (int i = 0; i < m_member->GetArity(); ++i)
    {
        if (i > 0) signature += ", ";
        signature += context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) +
            " " + m_member->GetArgumentInfo(i)->GetName();
    }
    signature += ")";
    return signature;

}

std::string MemberLoggerImpl::CreateConstructorSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("");
    std::string returnType = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
    if (returnType.size() > 0)
    {
        //returnType = returnType.substr(0,returnType.find_last_of("*")-1);
        signature += returnType + "(";
    }
    else
    {
        // Should never get here!
        signature += "(";
    }

    for (int i = 0; i < m_member->GetArity(); ++i)
    {
        if (i > 0) signature += ", ";
        signature += context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) +
            " " + m_member->GetArgumentInfo(i)->GetName();
    }
    signature += ")";
    return signature;
}

std::string MemberLoggerImpl::CreatePropertyGetSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("");
    std::string returnType = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
    if (returnType.size() > 0)
    {
        signature += returnType + "()";
    }
    else
    {
        // Should never get here!
        signature += "void()";
    }
    return signature;
}

std::string MemberLoggerImpl::CreatePropertySetSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("void(");
    for (int i = 1; i < m_member->GetArity(); ++i)
    {
        if (i > 1) signature += ", ";
        signature += context->GetTypeFormatter()->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) +
            " " + m_member->GetArgumentInfo(i)->GetName();
    }
    signature += ")";
    return signature;
}

std::string MemberLoggerImpl::CreateOperatorSignature(MemberLoggerContext* context /*= nullptr*/) const
{
    std::string signature("");
    std::string returnType = context->GetTypeFormatter()->FormatType(m_member->GetReturnType());
    if (returnType.size() > 0)
    {
        signature += returnType + "()";
    }
    else
    {
        // Should never get here!
        signature += "void()";
    }
    return signature;
}

}}}}
