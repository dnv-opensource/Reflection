//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionDetails.h"
#include <numeric>



namespace DNVS { namespace MoFa { namespace TypeUtilities {

    FunctionDetails::FunctionDetails(const std::string& dllName, const std::string& className, const std::string& functionName, const std::string& signature) 
        :   m_dllName(dllName)
        ,   m_className(className)
        ,   m_functionName(functionName)
        ,   m_signature(signature)
    {
    }

    void FunctionDetails::AddCall(double duration)
    {
        m_calls.insert(duration);
    }

    const std::string& FunctionDetails::GetDllName() const 
    {
        return m_dllName;
    }

    const std::string& FunctionDetails::GetClassName() const 
    {
        return m_className;
    }

    const std::string& FunctionDetails::GetFunctionName() const 
    {
        return m_functionName;
    }

    const std::string& FunctionDetails::GetSignature() const 
    {
        return m_signature;
    }

    size_t FunctionDetails::GetUseCount() const 
    {
        return m_calls.size();
    }

    double FunctionDetails::GetTotalDuration() const 
    {
        return std::accumulate(m_calls.begin(), m_calls.end(), 0.0);
    }

    double FunctionDetails::GetMaxDuration() const 
    {
        if(m_calls.empty())
            return 0.0;
        else
            return *m_calls.rbegin();
    }

}}}