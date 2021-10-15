//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsMemberLogger.h"
#include "Services\Module.h"
#include "Services\ServiceProvider.h"
#include "TypeUtilities\FunctionDetails\IFunctionDetailService.h"
#include "TypeUtilities\FunctionDetails\IFunctionDetails.h"
#include "jsMember.h"
#include "jsStack.h"
#include "boost/algorithm/string/replace.hpp"
#include "jsConstructor.h"
#include "jsProperty.h"


jsMemberLogger::jsMemberLogger(jsMember* member, bool logCall)
    :   m_member(member)
    ,   m_start(clock())
    ,   m_service(DNVS::MoFa::Services::ServiceProvider::Instance().TryGetService<DNVS::MoFa::TypeUtilities::IFunctionDetailService>())
    ,   m_logCall(logCall)
{    
    Initialize();
}

jsMemberLogger::jsMemberLogger(jsMember* member, const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service, bool logCall)
    :   m_member(member)
    ,   m_service(service)
    ,   m_logCall(logCall)
    ,   m_start(clock())
{
    Initialize();
}

void jsMemberLogger::Initialize()
{
    if(m_service)
    {
        using namespace DNVS::MoFa::Services;
        m_dll = Module(Detail::ModuleHandle(typeid(*m_member))).FileName();

        if(m_member->class_type()!=typeid(void))
            m_class = jsStack::stack()->typeName(m_member->class_type());        
        if(m_class.empty())
        {
            jsConstructor* constructor = dynamic_cast<jsConstructor*>(m_member);
            if(constructor)
                m_class = constructor->return_value();
        }
        if(m_member->typeInfo())
            m_function = m_member->typeInfo()->name();
        m_signature = CreateSignature();
        boost::replace_all(m_class, ">", "_");
        boost::replace_all(m_class, "<", "_");
    }
}

jsMemberLogger::~jsMemberLogger()
{
    if(m_service && !std::uncaught_exception())
    {
		std::lock_guard<std::mutex> guard(m_service->GetMutex());
        auto function = m_service->GetOrCreateFunctionDetails(m_dll, m_class, m_function, m_signature);
        if(function && m_logCall)
            function->AddCall(double(clock()-m_start)/CLOCKS_PER_SEC);
    }
}

std::string jsMemberLogger::CreateSignature() const
{
    std::string signature("");

    if (dynamic_cast<jsPropertyGet*>(m_member))
    {
        if (m_member->typeInfo() && m_member->typeInfo()->returned())
            signature = m_member->typeInfo()->returned()->name() + "()";
        return signature;
    }
    else if (dynamic_cast<jsPropertyPut*>(m_member))
    {
        return "void" + AddArguments();
    }
    else if (m_member->typeInfo() && m_member->typeInfo()->returned())
    {
        signature = m_member->typeInfo()->returned()->name();
    }

    signature += AddArguments();

    boost::replace_all(signature, "<", "_");
    boost::replace_all(signature, ">", "_");
    return signature;
}

std::string jsMemberLogger::AddArguments() const
{
    std::string signature = "(";
    for (int i = 0; i < m_member->param_size(); ++i)
    {
        if (i > 0) signature += ", ";
        signature += m_member->param_value(i);
    }
    signature += ")";
    return signature;
}
