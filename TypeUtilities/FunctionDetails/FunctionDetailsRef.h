#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFunctionDetails.h"
#include <set>

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class FunctionDetailsRef : public IFunctionDetails
    {
    public:
        FunctionDetailsRef(const std::string& dllName, const std::string& className, const std::string& functionName, const std::string& signature)
            :   m_dllName(dllName)
            ,   m_className(className)
            ,   m_functionName(functionName)
            ,   m_signature(signature)
        {}
        ///Add a single invocation of this call with the duration it took.
        virtual void AddCall(double duration) override { throw std::runtime_error(__FUNCTION__ " is not defined");}
        ///Gets the name of the DLL in which this class lies.
        virtual const std::string& GetDllName() const override { return m_dllName; }
        ///Gets the name of the class in which this function belongs, if any. Returns blank if this is a free function.
        virtual const std::string& GetClassName() const override { return m_className; }
        ///Gets the name of this function
        virtual const std::string& GetFunctionName() const override { return m_functionName; }
        ///Gets the function signature, such as '(int, double)' or 'getter' or 'setter'
        virtual const std::string& GetSignature() const override { return m_signature; }
        ///Gets the number of times this function has been called.
        virtual size_t GetUseCount() const override { throw std::runtime_error(__FUNCTION__ " is not defined");}
        ///Gets the sum of all the durations this function has been running.
        virtual double GetTotalDuration() const override { throw std::runtime_error(__FUNCTION__ " is not defined");}
        ///Gets the longest duration this function has been running.
        virtual double GetMaxDuration() const override { throw std::runtime_error(__FUNCTION__ " is not defined");}
    private:
        const std::string& m_dllName;
        const std::string& m_className;
        const std::string& m_functionName;
        const std::string& m_signature;
    };
}}}