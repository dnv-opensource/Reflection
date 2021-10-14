#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IFunctionDetailService.h"
#include "LessFunctionDetails.h"
#include <set>
#include <time.h>
#include <iosfwd>
#include "TypeUtilities/AutoLink.h"

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    
    class FunctionDetailService : public IFunctionDetailService
    {
    public:
        FunctionDetailService();
        ~FunctionDetailService();
        virtual std::shared_ptr<IFunctionDetails> GetOrCreateFunctionDetails(const std::string& dllName, const std::string& className, const std::string& functionName, const std::string& signatureName);
        virtual void Log(std::ostream& stream, const std::string& workspaceName);
        virtual void LogToCSV(std::ostream& stream, const std::string& workspaceName);
        virtual void Clear();
        virtual void Start();

		std::mutex& GetMutex() override;

	private:
        double ComputeCoverage() const;
        clock_t m_start;
        std::set<std::shared_ptr<IFunctionDetails>, LessFunctionDetails> m_functionDetails;
		std::mutex m_mutex;
    };

}}}