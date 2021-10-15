#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Config.h"
#include "IPlugin.h"
#include <memory>

namespace DNVS { namespace MoFa { namespace Plugins {
    class PLUGINS_IMPORT_EXPORT IPluginManager {
    public:
        IPluginManager();
        virtual ~IPluginManager();
        virtual void AddPlugin(const std::string& name) = 0;
        virtual std::list<std::shared_ptr<ISubsystem> > GetSubsystems() = 0;
    };

    template<typename SubsystemType>
    std::list<std::shared_ptr<SubsystemType>> GetSubsystems(std::shared_ptr<IPluginManager> manager)
    {
        if(!manager)
            throw std::runtime_error("Plugin manager not initialized!");

        std::list<std::shared_ptr<SubsystemType>> specializedSubsystems;
        auto subsystems = manager->GetSubsystems();
        for(auto it = subsystems.begin(); it != subsystems.end(); ++it)
        {
            auto subsystem = std::dynamic_pointer_cast<SubsystemType>(*it);
            if(subsystem)
            {
                specializedSubsystems.push_back(subsystem);
            }
        }
        return specializedSubsystems;
    }
}}}