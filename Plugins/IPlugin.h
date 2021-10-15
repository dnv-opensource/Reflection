#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <string>
#include <list>

namespace DNVS { namespace MoFa { namespace Plugins {
    class IPluginContext;
    class ISubsystem;
    ///Each DLL that wants to have plugin support
    class IPlugin 
    {
    public:
        virtual ~IPlugin() {}
        ///Returns a list of DLLs that needs to be loaded.
        virtual std::list<std::string> GetDependencies() const = 0;
        virtual std::list<std::shared_ptr<ISubsystem>> GetSubsystems() const = 0;
    };
}}}