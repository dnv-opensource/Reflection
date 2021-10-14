#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFunctionDetails.h"
#include "TypeUtilities/AutoLink.h"
#include <set>

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class FunctionDetails : public IFunctionDetails
    {
    public:
        FunctionDetails(const std::string& dllName, const std::string& className, const std::string& functionName, const std::string& signature);
        ///Add a single invocation of this call with the duration it took.
        virtual void AddCall(double duration) override;
        ///Gets the name of the DLL in which this class lies.
        virtual const std::string& GetDllName() const override;
        ///Gets the name of the class in which this function belongs, if any. Returns blank if this is a free function.
        virtual const std::string& GetClassName() const override;
        ///Gets the name of this function
        virtual const std::string& GetFunctionName() const override;
        ///Gets the function signature, such as '(int, double)' or 'getter' or 'setter'
        virtual const std::string& GetSignature() const override;
        ///Gets the number of times this function has been called.
        virtual size_t GetUseCount() const override;
        ///Gets the sum of all the durations this function has been running.
        virtual double GetTotalDuration() const override;
        ///Gets the longest duration this function has been running.
        virtual double GetMaxDuration() const override;
    private:
        std::string m_dllName;
        std::string m_className;
        std::string m_functionName;
        std::string m_signature;
        std::multiset<double> m_calls;
    };
}}}