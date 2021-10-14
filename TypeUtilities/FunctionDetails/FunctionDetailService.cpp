//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionDetailService.h"
#include "FunctionDetailsRef.h"
#include "FunctionDetails.h"

namespace DNVS { namespace MoFa { namespace TypeUtilities {

    FunctionDetailService::FunctionDetailService()
    {
        m_start = 0;
    }

    std::shared_ptr<IFunctionDetails> FunctionDetailService::GetOrCreateFunctionDetails(const std::string& dllName, const std::string& className, const std::string& functionName, const std::string& signatureName)
    {
        if(m_start == 0)
            m_start = clock();
        auto it = m_functionDetails.find(std::make_shared<FunctionDetailsRef>(dllName, className, functionName, signatureName));
        if(it == m_functionDetails.end())
            it = m_functionDetails.insert(std::make_shared<FunctionDetails>(dllName, className, functionName, signatureName)).first;
        return *it;
    }
    
    double FunctionDetailService::ComputeCoverage() const
    {
        size_t usedFunctions = 0;
        for(auto it = m_functionDetails.begin(); it != m_functionDetails.end(); ++it)
        {
            const std::shared_ptr<IFunctionDetails>& details = *it;
            if(details->GetUseCount() > 0)
                ++usedFunctions;
        }
        return double(usedFunctions)/m_functionDetails.size();
    }

    void FunctionDetailService::Log(std::ostream& stream, const std::string& workspaceName)
    {


        clock_t end = clock();
        FunctionDetails totalRunTime("Total Run Time", "", "", "");
        totalRunTime.AddCall(double(end-m_start)/CLOCKS_PER_SEC);
        stream << "<?xml version=\"1.0\" encoding=\"ASCII\"?>" << std::endl;

        stream << "<workspace ";
        stream << "name=\"" << workspaceName << "\" ";
        stream << "totalCoverage=\"" << ComputeCoverage() * 100 << "%\"";
        stream << ">" << std::endl;
        stream << "    " << totalRunTime;
        for(auto it = m_functionDetails.begin(); it != m_functionDetails.end(); ++it)
        {
            stream << "    " << **it;
        }
        stream << "</workspace>" << std::endl;
    }

    void FunctionDetailService::LogToCSV(std::ostream& stream, const std::string& workspaceName)
    {
        std::string separator(";");
        stream << "DLL name" << separator << "Class name" << separator << "Scripting command" << separator 
            << "Signature" << separator << "Use count" << separator << "Total duration" << separator 
            << "Max duration" << std::endl;
        for(auto it = m_functionDetails.begin(); it != m_functionDetails.end(); ++it)
        {
            auto details = *it;
            stream << details->GetDllName() << separator << details->GetClassName() << separator << details->GetFunctionName() << separator 
                << details->GetSignature() << separator << details->GetUseCount() << separator << details->GetTotalDuration() << separator 
                << details->GetMaxDuration() << std::endl;
        }
    }

    void FunctionDetailService::Clear()
    {
        m_functionDetails.clear();
        m_start = 0;
    }

    FunctionDetailService::~FunctionDetailService()
    {
    }

    void FunctionDetailService::Start()
    {
        m_start = clock();
    }

	std::mutex& FunctionDetailService::GetMutex()
	{
		return m_mutex;
	}

}}}

