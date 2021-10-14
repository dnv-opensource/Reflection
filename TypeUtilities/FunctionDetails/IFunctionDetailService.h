#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <string>
#include <mutex>


namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class IFunctionDetails;
    class IFunctionDetailService
    {
    public:
        virtual ~IFunctionDetailService() {}
        virtual std::shared_ptr<IFunctionDetails> GetOrCreateFunctionDetails(const std::string& dllName, const std::string& className, const std::string& functionName, const std::string& signatureName) = 0;
        ///Dump a log to stream, xml formatted
        virtual void Log(std::ostream& stream, const std::string& workspaceName) = 0;
        ///Dump a log to stream, csv (comma-separated values) formatted
        virtual void LogToCSV(std::ostream& stream, const std::string& workspaceName) = 0;
        ///Starts the clock. Called when opening a workspace.
        virtual void Start() = 0;
        ///Clears all collected data and resets the clock. Called after model has been saved.
        virtual void Clear() = 0;
		virtual std::mutex& GetMutex() = 0;
    };
}}}